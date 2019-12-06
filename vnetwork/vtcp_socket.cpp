#include "vtcp_socket.h"

#include <atomic>

#include "impl_vposix/wrap_errno.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/wrap_sys_socket.h"

#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
//EINPROGRESS
//              The socket is nonblocking and the connection cannot be
//              completed immediately.  (UNIX domain sockets failed with
//              EAGAIN instead.)  It is possible to select(2) or poll(2) for
//              completion by selecting the socket for writing.  After
//              select(2) indicates writability, use getsockopt(2) to read the
//              SO_ERROR option at level SOL_SOCKET to determine whether
//              connect() completed successfully (SO_ERROR is zero) or
//              unsuccessfully (SO_ERROR is one of the usual error codes
//              listed here, explaining the reason for the failure).
//=======================================================================================


//=======================================================================================
//      pimpl
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class vtcp_socket::_pimpl : epoll_receiver
{
public:
    _pimpl( vtcp_socket *owner_ );

    ~_pimpl() override;

    void on_many_flags( epoll_receiver::events ev ) override;

    void on_ready_read()             override;
    void on_ready_write()            override;
    void on_peer_shut_down_writing() override;

    void close();

    void connect( const vsocket_address& addr );

    bool shutdown_read();
    bool shutdown_write();
    bool shutdown_rw();

private:
    enum Stage { Unconnected, WaitConnection, Connected, Corrupted };

    Stage stage = Unconnected;

    safe_fd fd;
    vtcp_socket *owner;
};
#pragma GCC diagnostic pop
//=======================================================================================
void vtcp_socket::_pimpl::connect( const vsocket_address& addr )
{
    if ( !addr.is_valid() ) throw verror << "Not valid socket address for connect.";

    fd = addr.is_ip4() ? wrap_sys_socket::socket_tcp_ip4()
                       : wrap_sys_socket::socket_tcp_ip6();

    bool conn_ok = wrap_sys_socket::connect_no_err( fd, addr.data(), addr.data_size() );

    //  Socket should be nonblock, so error must be EINPROGRESS
    if ( conn_ok ) throw verror << "Bad connect logic!";

    ErrNo err;
    if ( !err.operation_in_progress() )
        throw verror << "Error during connect op: '" << err.text() << "'.";

    wrap_sys_socket::set_out_of_band_data( fd, true );
    fd.poll_add_rw( this );

    stage = WaitConnection;
}
//---------------------------------------------------------------------------------------
vtcp_socket::_pimpl::_pimpl( vtcp_socket *owner_ )
    : owner ( owner_ )
{}
//---------------------------------------------------------------------------------------
vtcp_socket::_pimpl::~_pimpl()
{
    if ( !fd.has() ) return;
    shutdown_rw();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::on_many_flags( epoll_receiver::events ev )
{
    //  Когда прилетает ошибка 111, в ней вот так флаги выставлены.
    if ( !ev.take_error()        )  throw verror("Bad logic");
    if ( !ev.take_read()         )  throw verror("Bad logic");
    if ( !ev.take_write()        )  throw verror("Bad logic");
    if ( !ev.take_hang_up()      )  throw verror("Bad logic");
    if ( !ev.take_read_hang_up() )  throw verror("Bad logic");
    ev.throw_if_need( "vtcp_socket::on_many_flags" );


    auto err = wrap_sys_socket::get_error( fd );
    fd.close();

    stage = Corrupted;

    if ( err.connect_refused() )
        owner->error_occured_code( err_codes::connection_refused );
    else
        vwarning << "Unknown tcp error code" << err.text();

    owner->error_occured_text( err.text() );
    owner->disconnected();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::on_ready_read()
{
    vdeb;
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::on_ready_write()
{
    if ( stage != WaitConnection )
        throw verror( "Bad logic, do not wait write signal" );

    fd.poll_mod_read( this );
    stage = Connected;

    owner->connected();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::on_peer_shut_down_writing()
{
    vdeb;
    owner->disconnected();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::close()
{
    stage = Unconnected;

    if ( !fd.has() ) return;

    shutdown_rw();
    fd.close();
}
//---------------------------------------------------------------------------------------
bool vtcp_socket::_pimpl::shutdown_read()
{
    return wrap_sys_socket::shutdown_rd_no_err( fd );
}
//---------------------------------------------------------------------------------------
bool vtcp_socket::_pimpl::shutdown_write()
{
    return wrap_sys_socket::shutdown_wr_no_err( fd );
}
//---------------------------------------------------------------------------------------
bool vtcp_socket::_pimpl::shutdown_rw()
{
    return wrap_sys_socket::shutdown_rdwr_no_err( fd );
}
//=======================================================================================
//      pimpl
//=======================================================================================


//=======================================================================================
//      vtcp_socket interface
//=======================================================================================
vtcp_socket::vtcp_socket()
    : _p( new _pimpl(this) )
{}
//---------------------------------------------------------------------------------------
vtcp_socket::~vtcp_socket()
{}
//---------------------------------------------------------------------------------------
void vtcp_socket::connect( const vsocket_address& addr )
{
    _p->connect( addr );
}
//---------------------------------------------------------------------------------------
void vtcp_socket::listen( const vsocket_address& addr )
{

}
//---------------------------------------------------------------------------------------
void vtcp_socket::listen_any( uint16_t port )
{

}
//---------------------------------------------------------------------------------------
void vtcp_socket::listen_loopback( uint16_t port )
{

}
//---------------------------------------------------------------------------------------
void vtcp_socket::close()
{
    if ( _p ) _p->close();
}
//---------------------------------------------------------------------------------------
bool vtcp_socket::shutdown_read()
{
    return _p->shutdown_read();
}
//---------------------------------------------------------------------------------------
bool vtcp_socket::shutdown_write()
{
    return _p->shutdown_write();
}
//---------------------------------------------------------------------------------------
bool vtcp_socket::shutdown_rw()
{
    return _p->shutdown_rw();
}
//=======================================================================================
//      vtcp_socket interface
//=======================================================================================


//=======================================================================================
//      vtcp_socket::accepted_socket
//=======================================================================================
vtcp_socket::accepted_socket::accepted_socket( int fd )
    : _p( std::make_shared<std::atomic<int>>(fd) )
{}
//---------------------------------------------------------------------------------------
vtcp_socket::accepted_socket::~accepted_socket()
{
    if ( !_p ) return;
    vwarning << "accepted_socket destroyed, but socket not used, will close forced";
    wrap_unistd::close_safe( take() );
}
//---------------------------------------------------------------------------------------
int vtcp_socket::accepted_socket::take()
{
    assert( _p );
    auto res = _p->fetch_or( -1 );
    _p.reset();
    return res;
}
//=======================================================================================
//      vtcp_socket::accepted_socket
//=======================================================================================
