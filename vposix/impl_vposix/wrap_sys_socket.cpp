#include "wrap_sys_socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include "impl_vposix/linux_call.h"


using namespace impl_vposix;


//=======================================================================================
//  protocol == 0 --> default for domain & type.
int _socket( int domain, int type )
{
    type |= SOCK_NONBLOCK | SOCK_CLOEXEC;

    return linux_call::check( ::socket, domain, type, 0 );
}
//---------------------------------------------------------------------------------------
safe_fd wrap_sys_socket::socket_tcp( int af_type )
{
    return _socket( af_type, SOCK_STREAM );
}
//---------------------------------------------------------------------------------------
safe_fd wrap_sys_socket::socket_tcp_ip4()
{
    return socket_tcp( AF_INET );
}
//---------------------------------------------------------------------------------------
safe_fd wrap_sys_socket::socket_tcp_ip6()
{
    return socket_tcp( AF_INET6 );
}
//---------------------------------------------------------------------------------------
safe_fd wrap_sys_socket::socket_udp( int af_type )
{
    return _socket( af_type, SOCK_DGRAM );
}
//---------------------------------------------------------------------------------------
safe_fd wrap_sys_socket::socket_udp_ip4()
{
    return socket_udp( AF_INET );
}
//---------------------------------------------------------------------------------------
safe_fd wrap_sys_socket::socket_udp_ip6()
{
    return socket_udp( AF_INET6 );
}
//=======================================================================================
//  If the connection or binding succeeds, zero is returned.  On error,
//  -1 is returned, and errno is set appropriately.
bool wrap_sys_socket::connect_no_err( int fd, const void *sa, unsigned sa_size )
{
    auto sa_ptr = static_cast<const sockaddr*>( sa );

    return 0 == linux_call::no_err( ::connect, fd, sa_ptr, sa_size );
}
//=======================================================================================
bool wrap_sys_socket::connect_ip4_no_err(int fd, const sockaddr_in *sa4 )
{
    return connect_no_err( fd, sa4, sizeof(sockaddr_in) );
}
//=======================================================================================
bool wrap_sys_socket::connect_ip6_no_err(int fd, const sockaddr_in6 *sa6 )
{
    return connect_no_err( fd, sa6, sizeof(sockaddr_in6) );
}
//=======================================================================================


//=======================================================================================
static void set_sock_opt( int fd, int level, int optname,
                          const void *optval, socklen_t optlen )
{
    linux_call::check( ::setsockopt, fd, level, optname, optval, optlen );
}
//---------------------------------------------------------------------------------------
static void set_sock_opt_int( int fd, int level, int optname, int val )
{
    set_sock_opt( fd, level, optname, &val, sizeof(val) );
}
//---------------------------------------------------------------------------------------
void wrap_sys_socket::set_out_of_band_data( int fd, bool set )
{
    set_sock_opt_int( fd, SOL_SOCKET, SO_OOBINLINE, set ? 1 : 0 );
}
//=======================================================================================
static bool shutdown_no_err( int fd, int dirs )
{
    return 0 == linux_call::no_err( ::shutdown, fd, dirs );
}
//---------------------------------------------------------------------------------------
bool wrap_sys_socket::shutdown_rd_no_err( int fd )
{
    return shutdown_no_err( fd, SHUT_RD );
}
//---------------------------------------------------------------------------------------
bool wrap_sys_socket::shutdown_wr_no_err( int fd )
{
    return shutdown_no_err( fd, SHUT_WR );
}
//---------------------------------------------------------------------------------------
bool wrap_sys_socket::shutdown_rdwr_no_err( int fd )
{
    return shutdown_no_err( fd, SHUT_RDWR );
}
//=======================================================================================
//static int get_sock_opt( int fd, int level, int optname,
//                         void *__restrict optval,
//                         socklen_t *__restrict optlen)
//{
//    linux_call::check( ::getsockopt, fd, level, optname, optval, optlen );
//}
//---------------------------------------------------------------------------------------
static int get_sock_opt_int( int fd, int level, int optname )
{
    int res;
    socklen_t len = sizeof( res );
    linux_call::check( ::getsockopt, fd, level, optname, &res, &len );
    assert( len == sizeof(res) );
    return res;
}
//---------------------------------------------------------------------------------------
ErrNo wrap_sys_socket::get_error( int fd )
{
    return get_sock_opt_int( fd, SOL_SOCKET, SO_ERROR );
}
//=======================================================================================
void wrap_sys_socket::listen( int fd )
{

}
//=======================================================================================
