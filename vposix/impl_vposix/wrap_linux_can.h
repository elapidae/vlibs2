#ifndef IMPL_VPOSIX_WRAP_LINUX_CAN_H
#define IMPL_VPOSIX_WRAP_LINUX_CAN_H

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class wrap_linux_can
    {
    public:
        static int socket_raw();

        static void bind_raw( int socket, unsigned ifindex );
        static void bind_raw( int socket, const std::string& ifname );
    };
    //===================================================================================
}
//=======================================================================================


#endif // IMPL_VPOSIX_WRAP_LINUX_CAN_H
