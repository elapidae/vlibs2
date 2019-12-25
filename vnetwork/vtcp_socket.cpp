#include "vtcp_socket.h"

#include <atomic>

#include "impl_vposix/safe_fd.h"
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
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wpadded"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class vtcp_socket::_pimpl : epoll_receiver
{
public:
    _pimpl( vtcp_socket *owner_ );
    _pimpl( vtcp_socket *owner_, accepted_peer *peer );

    ~_pimpl() override;

    void on_events( events e ) override;

    void when_error( events e );

    void when_ready_read();
    void when_ready_write();
    //void on_peer_shut_down_writing();
    //void on_connect_refused();

    void close();

    void connect( const vsocket_address& addr );

    bool send( const std::string& data );

    void shutdown();

    bool is_connected() const;

    void keep_alive( int idle, int intvl, int cnt );

    vsocket_address address() const;
    vsocket_address peer_address() const;

private:
    vtcp_socket *owner;
    safe_fd fd;

    //  Пока непонятно надо ли вот это хранить.
    enum Stage { Unconnected, WaitConnection, Connected };
    Stage stage = Unconnected;
};
#pragma GCC diagnostic pop
//=======================================================================================
bool vtcp_socket::_pimpl::is_connected() const
{
    return fd.has() && stage == Connected;
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::keep_alive( int idle, int intvl, int cnt )
{
    wrap_sys_socket::keep_alive( fd, idle, intvl, cnt );
}
//---------------------------------------------------------------------------------------
vsocket_address vtcp_socket::_pimpl::address() const
{
    vsocket_address res;

    if ( !fd.has() )
        return res;

    wrap_sys_socket::get_sockaddr( fd, res._data(), res._data_size() );
    return res;
}
//---------------------------------------------------------------------------------------
vsocket_address vtcp_socket::_pimpl::peer_address() const
{
    vsocket_address res;

    if ( !is_connected() )
        return res;

    wrap_sys_socket::get_peeraddr( fd, res._data(), res._data_size() );
    return res;
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::connect( const vsocket_address& addr )
{
    if ( !addr.is_valid() ) throw verror << "Not valid socket address for connect.";

    fd = wrap_sys_socket::socket_tcp( addr._family() );

    bool conn_ok = wrap_sys_socket::connect_no_err(fd, addr._data(), addr._data_size());

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
bool vtcp_socket::_pimpl::send( const std::string& data )
{
    if ( !is_connected() )
    {
        vwarning << "Cannot send to unconnected socket";
        return false;
    }

    return wrap_sys_socket::send_no_err( fd, data );
}
//---------------------------------------------------------------------------------------
vtcp_socket::_pimpl::_pimpl( vtcp_socket *owner_ )
    : owner ( owner_      )
    , stage ( Unconnected )
{}
//---------------------------------------------------------------------------------------
vtcp_socket::_pimpl::_pimpl( vtcp_socket *owner_, accepted_peer *peer )
    : owner ( owner_    )
    , stage ( Connected )
{
    fd = peer->_take_fd();
    fd.poll_add_read( this );
}
//---------------------------------------------------------------------------------------
vtcp_socket::_pimpl::~_pimpl()
{
    if ( fd.has() ) shutdown();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::on_events( epoll_receiver::events e )
{
    if ( e.take_error() )
    {
        when_error( e );
        return;
    }

    // When some data received.
    if ( e.take_read() )
        when_ready_read();

    // When connected to server.
    if ( e.take_write() )
        when_ready_write();

    // Normal finalization of connection.
    if ( e.take_read_hang_up() )
    {
        fd.close();
        stage = Unconnected;
        owner->disconnected();
        return;
    }
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::when_ready_write()
{
    if ( stage != WaitConnection )
        throw verror( "Bad logic, do not wait write signal" );

    fd.poll_mod_read( this );
    stage = Connected;

    owner->connected();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::when_error( epoll_receiver::events e )
{
    auto err = wrap_sys_socket::get_error( fd );

    fd.close();
    auto err_stage = stage;
    stage = Unconnected;
    owner->disconnected();

    if ( err_stage == Connected )
    {
        if ( err.broken_pipe() )
            owner->err_broken_pipe();

        return;
    } // Connected

    if ( err_stage == WaitConnection )
    {
        if ( err.connect_refused() )
            owner->err_conn_refused();

        return;
    } // WaitConnection

    //  For debugging...
    vdeb << "R"   << e.take_read();
    vdeb << "W"   << e.take_write();
    vdeb << "HU"  << e.take_hang_up();
    vdeb << "RHU" << e.take_read_hang_up();
    vdeb << "err" << err;
    e.check_empty();

    throw verror;
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::when_ready_read()
{
    enum { _buffer_size = 4096 };

    char buf[ _buffer_size ];
    std::string res;

    while(1)
    {
        auto has_read = wrap_sys_socket::recv_no_err( fd, buf, _buffer_size );

        if ( has_read == -1 )
        {
            ErrNo err;
            if ( err.resource_unavailable_try_again() )
                break;

            err.do_throw( "vtcp_socket::_pimpl::on_ready_read" );
        }

        res.append( buf, size_t(has_read) );

        if ( has_read < _buffer_size )
            break;
    }

    if ( !res.empty() )
        owner->received( std::move(res) );
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::close()
{
    stage = Unconnected;

    if ( !fd.has() ) return;

    shutdown();
    fd.close();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::_pimpl::shutdown()
{
    wrap_sys_socket::shutdown_rdwr_no_err( fd );
}
//=======================================================================================
//      pimpl
//=======================================================================================


//=======================================================================================
//      vtcp_socket interface
//=======================================================================================
vtcp_socket::unique_ptr vtcp_socket::make_unique( accepted_peer *peer )
{
    return unique_ptr{ new vtcp_socket(peer) };
}
//---------------------------------------------------------------------------------------
vtcp_socket::shared_ptr vtcp_socket::make_shared( accepted_peer *peer )
{
    return std::make_shared<vtcp_socket>( peer );
}
//=======================================================================================
vtcp_socket::vtcp_socket()
    : _p( new _pimpl(this) )
{}
//---------------------------------------------------------------------------------------
vtcp_socket::vtcp_socket( accepted_peer *peer )
    : _p( new _pimpl(this, std::move(peer)) )
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
bool vtcp_socket::is_connected() const
{
    return _p->is_connected();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::send( const std::string& data )
{
    _p->send( data );
}
//---------------------------------------------------------------------------------------
void vtcp_socket::keep_alive( int idle, int intvl, int cnt )
{
    _p->keep_alive( idle, intvl, cnt );
}
//---------------------------------------------------------------------------------------
vsocket_address vtcp_socket::address() const
{
    return _p->address();
}
//---------------------------------------------------------------------------------------
vsocket_address vtcp_socket::peer_address() const
{
    return _p->peer_address();
}
//---------------------------------------------------------------------------------------
void vtcp_socket::close()
{
    _p->close();
}
//=======================================================================================
//      vtcp_socket interface
//=======================================================================================


//=======================================================================================
//      vtcp_socket::accepted_socket
//=======================================================================================
vtcp_socket::accepted_peer::accepted_peer( int fd, vsocket_address && peer_addr )
    : _fd  ( std::make_shared<int>(fd) )
    , _peer( std::move(peer_addr)      )
{}
//---------------------------------------------------------------------------------------
vtcp_socket::unique_ptr vtcp_socket::accepted_peer::as_unique()
{
    return vtcp_socket::make_unique( this );
}
//---------------------------------------------------------------------------------------
vtcp_socket::shared_ptr vtcp_socket::accepted_peer::as_shared()
{
    return vtcp_socket::make_shared( this );
}
//---------------------------------------------------------------------------------------
const vsocket_address &vtcp_socket::accepted_peer::peer_address() const
{
    return _peer;
}
//---------------------------------------------------------------------------------------
int vtcp_socket::accepted_peer::_take_fd()
{
    assert( _fd );
    auto res = *_fd.get();
    _fd.reset();
    return res;
}
//=======================================================================================
//      vtcp_socket::accepted_socket
//=======================================================================================
