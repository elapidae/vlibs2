#ifndef IMPL_VPOSIX_WRAP_SYS_SOCKET_H
#define IMPL_VPOSIX_WRAP_SYS_SOCKET_H

#include "impl_vposix/wrap_errno.h"
#include "impl_vposix/wrap_unistd.h"

//=======================================================================================
/*
 *  ::connect:      http://man7.org/linux/man-pages/man2/connect.2.html
 *
*/
//  Mmm, bad, need predeclare.
struct sockaddr;
struct sockaddr_in;
struct sockaddr_in6;
//=======================================================================================

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_sys_socket
    {
        static safe_fd socket_tcp( int af_type );
        static safe_fd socket_tcp_ip4();
        static safe_fd socket_tcp_ip6();

        static safe_fd socket_udp( int af_type );
        static safe_fd socket_udp_ip4();
        static safe_fd socket_udp_ip6();

        static bool connect_no_err( int fd, const void* sa, unsigned sa_size );
        static bool connect_ip4_no_err( int fd, const sockaddr_in  *sa4 );
        static bool connect_ip6_no_err( int fd, const sockaddr_in6 *sa6 );
        
        //  Внеполосные данные, та еще трава.
        static void set_out_of_band_data( int fd, bool set = true );

        static bool shutdown_rd_no_err   ( int fd );
        static bool shutdown_wr_no_err   ( int fd );
        static bool shutdown_rdwr_no_err ( int fd );

        static ErrNo get_error(int fd);

        static void listen( int fd );
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_SYS_SOCKET_H
