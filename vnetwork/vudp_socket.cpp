#include "vudp_socket.h"

#include <vector>

#include "impl_vposix/safe_fd.h"
#include "impl_vposix/wrap_sys_socket.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
class vudp_socket::_pimpl : epoll_receiver
{
public:
    _pimpl( vudp_socket *owner_ );
    ~_pimpl() override;

    void bind( const vsocket_address& addr );
    void close();

    void send_to( const vsocket_address& addr, const std::string& data );

    vsocket_address address() const;
    bool is_binded() const;

private:
    void on_events(events) override;
    bool receive();

    vudp_socket *owner;
    safe_fd fd;
    std::vector<char> buffer;
};
//=======================================================================================
vudp_socket::_pimpl::_pimpl( vudp_socket *owner_ )
    : owner( owner_ )
{}
//=======================================================================================
vudp_socket::_pimpl::~_pimpl()
{
    wrap_sys_socket::shutdown_rdwr_no_err( fd );
}
//=======================================================================================
void vudp_socket::_pimpl::bind( const vsocket_address& addr )
{
    fd = wrap_sys_socket::socket_udp( addr._family() );

    //  Не уверен по опциям, надо будет разбираться по ходу появления неприятностей.
    //wrap_sys_socket::set_broadcast( fd );
    //Socket::set_ip_receive_packet_information( fd );
    //Socket::set_ip_receive_hop_limit( fd );

    wrap_sys_socket::bind( fd, addr._data(), addr._data_size() );

    fd.poll_add_read( this );
}
//=======================================================================================
void vudp_socket::_pimpl::close()
{
    fd.close();
}
//=======================================================================================
void vudp_socket::_pimpl::send_to( const vsocket_address& addr, const std::string& data )
{
    if ( !is_binded() )
    {
        if      (addr.is_ip4()) bind( vsocket_address::any_ip4() );
        else if (addr.is_ip6()) bind( vsocket_address::any_ip6() );
        else    throw verror("UDP: try to send to invalid socket.");
    }

    wrap_sys_socket::send_to( fd, data, addr._data(), addr._data_size() );
}
//=======================================================================================
vsocket_address vudp_socket::_pimpl::address() const
{
    vsocket_address res;
    wrap_sys_socket::get_sockaddr( fd, res._data(), res._data_size() );
    return res;
}
//=======================================================================================
bool vudp_socket::_pimpl::is_binded() const
{
    return fd.has();
}
//=======================================================================================
void vudp_socket::_pimpl::on_events( epoll_receiver::events e )
{
    // no errors checking, while...

    e.take_read();
    e.check_empty();

    while( receive() )
    {} // do nothing
}
//=======================================================================================
bool vudp_socket::_pimpl::receive()
{
    auto next_size = wrap_sys_socket::pending_datagram_size( fd );
    if ( next_size == 0 ) return false;

    if ( buffer.size() < next_size )
        buffer.resize( next_size );

    vsocket_address peer;
    auto real_size = wrap_sys_socket::recv_from( fd, buffer.data(), next_size,
                                                 peer._data(), peer._data_size() );
    assert( real_size == next_size );

    std::string data{ buffer.data(), real_size };
    owner->received( {std::move(data), std::move(peer)} );

    return true;
}
//=======================================================================================


//=======================================================================================
vudp_socket::unique_ptr vudp_socket::make_unique( const vsocket_address& addr )
{
    auto res = unique_ptr( new vudp_socket );
    res->bind( addr );
    return res;
}
//=======================================================================================
vudp_socket::shared_ptr vudp_socket::make_shared( const vsocket_address& addr )
{
    return std::make_shared<vudp_socket>( addr );
}
//=======================================================================================
vudp_socket::vudp_socket()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vudp_socket::vudp_socket( const vsocket_address& addr )
    : _p( new _pimpl(this) )
{
    bind( addr );
}
//=======================================================================================
vudp_socket::~vudp_socket()
{}
//=======================================================================================
void vudp_socket::bind( const vsocket_address& addr )
{
    _p->bind( addr );
}
//=======================================================================================
void vudp_socket::bind_any_ip4( uint16_t port )
{
    _p->bind( vsocket_address::any_ip4(port) );
}
//=======================================================================================
void vudp_socket::bind_any_ip6( uint16_t port )
{
    _p->bind( vsocket_address::any_ip6(port) );
}
//=======================================================================================
void vudp_socket::bind_loopback_ip4( uint16_t port )
{
    _p->bind( vsocket_address::loopback_ip4(port) );
}
//=======================================================================================
void vudp_socket::bind_loopback_ip6( uint16_t port )
{
    _p->bind( vsocket_address::loopback_ip6(port) );
}
//=======================================================================================
void vudp_socket::close()
{
    _p->close();
}
//=======================================================================================
void vudp_socket::send_to( const vsocket_address& addr, const std::string& data )
{
    _p->send_to( addr, data );
}
//=======================================================================================
vsocket_address vudp_socket::address() const
{
    if ( !is_binded() ) throw verror << "Cannot get addres from unbinded udp socket.";
    return _p->address();
}
//=======================================================================================
bool vudp_socket::is_binded() const
{
    return _p->is_binded();
}
//=======================================================================================
