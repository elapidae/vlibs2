#include "wrap_linux_can.h"

#include <sys/socket.h>
#include <linux/can.h>
#include <type_traits>

#include "impl_vposix/wrap_sys_socket.h"
#include "impl_vposix/wrap_net_if.h"

using namespace impl_vposix;

//=======================================================================================
template <typename DstType,typename SrcType>
static DstType two_cast(SrcType* ptr)
{
    return static_cast<DstType*>( static_cast<void*>(ptr) );
}
//=======================================================================================

//=======================================================================================
int wrap_linux_can::socket_raw()
{
    return wrap_sys_socket::socket( PF_CAN, SOCK_RAW, CAN_RAW );
}
//=======================================================================================
void wrap_linux_can::bind_raw( int socket, unsigned ifindex )
{
    struct sockaddr_can addr;
    addr.can_family  = AF_CAN;
    addr.can_ifindex = ifindex;

    wrap_sys_socket::bind( socket, &addr, sizeof(addr) );
}
//=======================================================================================
void wrap_linux_can::bind_raw( int socket, const std::string& ifname )
{
    auto index = ifname.empty()
                        ? 0
                        : wrap_net_if::name_to_index( ifname );

    bind_raw( socket, index );
}
//=======================================================================================
