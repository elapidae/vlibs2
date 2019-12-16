#ifndef IMPL_VPOSIX_WRAP_SYS_SOCKET_H
#define IMPL_VPOSIX_WRAP_SYS_SOCKET_H

#include "impl_vposix/wrap_errno.h"

//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man2/connect.2.html
 *  http://man7.org/linux/man-pages/man2/bind.2.html
 *  http://man7.org/linux/man-pages/man2/accept.2.html\
 *  https://linux.die.net/man/2/send
*/
//=======================================================================================

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_sys_socket
    {
        static int socket_tcp( int af_type );
        static int socket_udp( int af_type );

        static bool connect_no_err( int fd, const void *sa, unsigned sa_size );
        
        //  Внеполосные данные, та еще трава.
        static void set_out_of_band_data( int fd, bool set = true );

        static void set_reuse_address( int fd );

        static bool shutdown_rd_no_err   ( int fd );
        static bool shutdown_wr_no_err   ( int fd );
        static bool shutdown_rdwr_no_err ( int fd );

        static ErrNo get_error( int fd );
        static bool  get_out_of_band_data( int fd );

        static void bind( int fd, const void *sa, unsigned size );

        static void listen( int fd, int backlog );

        static int accept_no_err( int fd , void *peer_sa, unsigned peer_size );

        static ssize_t recv_no_err( int fd, void* buf, size_t len, int flags = 0 );

        static bool send_no_err( int fd, const std::string* data );

        static void keep_alive( int fd, int idle, int intvl, int cnt );
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_SYS_SOCKET_H
