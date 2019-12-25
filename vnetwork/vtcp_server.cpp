#include "vtcp_server.h"

#include "impl_vposix/safe_fd.h"
#include "impl_vposix/wrap_sys_epoll.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/wrap_sys_socket.h"

#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
class vtcp_server::_pimpl : epoll_receiver
{
public:
    _pimpl( vtcp_server *owner );

    void listen( const vsocket_address& addr );

    void on_events( events e) override;

    vsocket_address address();

private:
    vtcp_server *owner;
    safe_fd fd;
};
//=======================================================================================
vtcp_server::_pimpl::_pimpl( vtcp_server *owner_ )
    : owner( owner_ )
{}
//=======================================================================================
void vtcp_server::_pimpl::listen( const vsocket_address& addr )
{
    fd = wrap_sys_socket::socket_tcp( addr._family() );
    fd.poll_add_read( this );

    wrap_sys_socket::set_out_of_band_data( fd );
    wrap_sys_socket::set_reuse_address( fd );

    wrap_sys_socket::bind( fd, addr._data(), addr._data_size() );

    wrap_sys_socket::listen( fd, _queue_connections );
}
//=======================================================================================
void vtcp_server::_pimpl::on_events( epoll_receiver::events e )
{
    e.take_read();
    e.check_empty();

    while (1)
    {
        vsocket_address peer;
        auto peer_fd = wrap_sys_socket::accept_no_err( fd, peer._data(),
                                                           peer._data_size() );

        if ( peer_fd < 0 )
        {
            ErrNo err;
            if ( !err.resource_unavailable_try_again() )
                err.do_throw( "vtcp_server::_pimpl::on_events: unexpected" );
            break;
        }
        vtcp_socket::accepted_peer acc = { peer_fd, std::move(peer) };
        owner->accepted( std::move(acc) );
    }
}
//=======================================================================================
vsocket_address vtcp_server::_pimpl::address()
{
    vsocket_address res;

    if ( !fd.has() )
        return res;

    wrap_sys_socket::get_sockaddr( fd, res._data(), res._data_size() );
    return res;
}
//=======================================================================================


//=======================================================================================
vtcp_server::vtcp_server()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vtcp_server::~vtcp_server()
{}
//=======================================================================================
void vtcp_server::listen( const vsocket_address& addr )
{
    _p->listen( addr );
}
//=======================================================================================
void vtcp_server::listen_any_ip4( uint16_t port )
{
    _p->listen( vsocket_address::any_ip4(port) );
}
//=======================================================================================
void vtcp_server::listen_any_ip6( uint16_t port )
{
    _p->listen( vsocket_address::any_ip6(port) );
}
//=======================================================================================
void vtcp_server::listen_loopback_ip4( uint16_t port )
{
    _p->listen( vsocket_address::loopback_ip4(port) );
}
//=======================================================================================
void vtcp_server::listen_loopback_ip6( uint16_t port )
{
    _p->listen( vsocket_address::loopback_ip6(port) );
}
//=======================================================================================
vsocket_address vtcp_server::address() const
{
    return _p->address();
}
//=======================================================================================
