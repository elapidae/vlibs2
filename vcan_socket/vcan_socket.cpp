#include "vcan_socket.h"

#include <linux/can.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "impl_vposix/safe_fd.h"
#include "impl_vposix/wrap_sys_epoll.h"
#include "impl_vposix/wrap_net_if.h"
#include "impl_vposix/wrap_linux_can.h"
#include "impl_vposix/wrap_sys_socket.h"

#include "vbyte_buffer.h"
#include "vlog.h"

using namespace impl_vposix;

using str  = std::string;
using cstr = const std::string&;

//=======================================================================================
class vcan_socket::_pimpl : public impl_vposix::epoll_receiver
{
public:
    _pimpl( vcan_socket * own ) : owner(own) {}

    void on_events( events ) override;
    void on_error();
    bool read_1();

    void raw( cstr iface );
    //  for BCM: s = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);

    void send( id_type id, const std::string& data );

private:
    vcan_socket *owner;
    safe_fd fd;
};
//=======================================================================================
void vcan_socket::_pimpl::on_events( events evs )
{
    if ( evs.take_error() )
    {
        on_error();
        return;
    }
    if ( evs.take_read() )
    {
        while ( read_1() )
        {} // nothing to do
    }
    evs.check_empty();
}
//=======================================================================================
bool vcan_socket::_pimpl::read_1()
{
    can_frame    frame;
    sockaddr_can addr;

    auto res = wrap_sys_socket::recv_from_no_err( fd, &frame, sizeof(frame),
                                                      &addr,  sizeof(addr) );
    if (res < 0)
    {
        ErrNo err;
        if ( err.resource_unavailable_try_again() )
            return false;
        err.do_throw( "vcan_socket::read_1" );
    }

    if ( addr.can_family != AF_CAN )
        throw verror << "Bad CAN familly: " << addr.can_family << " (must be AF_CAN).";

    auto ptr = static_cast<char*>( static_cast<void*>(frame.data) );
    std::string data( ptr, frame.can_dlc );

    message msg
    {
        frame.can_id,
        { ptr, frame.can_dlc },
        addr.can_ifindex,
        addr.can_addr.tp.rx_id,
        addr.can_addr.tp.tx_id
    };

    owner->received( msg );
    return true;
}
//=======================================================================================
void vcan_socket::_pimpl::on_error()
{
    throw verror << "CAN error";
}
//=======================================================================================
void vcan_socket::_pimpl::raw( cstr iface )
{
    fd = wrap_linux_can::socket_raw();

    wrap_linux_can::bind_raw( fd, iface );

    fd.poll_add_read( this );
}
//=======================================================================================
void vcan_socket::_pimpl::send( id_type id, const std::string& data )
{
    if (data.size() > 8) throw verror << "Too big data";

    can_frame frame{};
    frame.can_id = id;
    frame.can_dlc = data.size();
    std::copy( data.begin(), data.end(), frame.data );

    wrap_sys_socket::send_raw( fd, &frame, sizeof(frame) );
}
//=======================================================================================
vcan_socket::vcan_socket()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vcan_socket::~vcan_socket()
{}
//=======================================================================================
void vcan_socket::bind( const std::string& iface )
{
    _p->raw( iface );
}
//=======================================================================================
void vcan_socket::send( id_type id, const std::string& data )
{
    _p->send( id, data );
}
//=======================================================================================


//=======================================================================================
std::ostream& operator <<( std::ostream& os, const vcan_socket::message& msg )
{
    os << std::hex << std::showbase
       << "|" << std::setw(6) << msg.id << " > "
       << std::setw(3*8) << vbyte_buffer(msg.data).to_Hex('.')
       << std::dec
       << ", if:"<< std::setw(3) << msg.iface
       << ", rx:"<< std::setw(3) << msg.rx
       << ", tx:"<< std::setw(3) << msg.tx
       << "|";

    return os;
}
//=======================================================================================
