#include "wrap_sys_socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <netinet/tcp.h>

#include "impl_vposix/linux_call.h"
#include "vlog.h"

using namespace impl_vposix;


//=======================================================================================
//  protocol == 0 --> default for domain & type.
int _socket( int domain, int type )
{
    type |= SOCK_NONBLOCK | SOCK_CLOEXEC;

    return linux_call::check( ::socket, domain, type, 0 );
}
//---------------------------------------------------------------------------------------
int wrap_sys_socket::socket_tcp( int af_type )
{
    return _socket( af_type, SOCK_STREAM );
}
//---------------------------------------------------------------------------------------
int wrap_sys_socket::socket_udp( int af_type )
{
    return _socket( af_type, SOCK_DGRAM );
}
//=======================================================================================
//  If the connection or binding succeeds, zero is returned.  On error,
//  -1 is returned, and errno is set appropriately.
bool wrap_sys_socket::connect_no_err(int fd, const void *sa, unsigned sa_size )
{
    auto sa_ptr = static_cast<const sockaddr*>( sa );
    return 0 == linux_call::no_err( ::connect, fd, sa_ptr, sa_size );
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
//---------------------------------------------------------------------------------------
void wrap_sys_socket::set_reuse_address( int fd )
{
    set_sock_opt_int( fd, SOL_SOCKET, SO_REUSEADDR, 1 );
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
bool wrap_sys_socket::get_out_of_band_data( int fd )
{
    return 1 == get_sock_opt_int( fd, SOL_SOCKET, SO_OOBINLINE );
}
//=======================================================================================
void wrap_sys_socket::bind( int fd, const void *sa, unsigned size )
{
    auto sa_ptr = static_cast<const sockaddr*>( sa );
    linux_call::check( ::bind, fd, sa_ptr, size );
}
//=======================================================================================
void wrap_sys_socket::listen( int fd, int backlog )
{
    linux_call::check( ::listen, fd, backlog );
}
//=======================================================================================
//  [1] The returned address is truncated if the buffer provided is too small;
//      in this case, addrlen will return a value greater than was supplied to the call.
//
int wrap_sys_socket::accept_no_err( int fd, void *peer_sa, unsigned peer_size )
{
    static constexpr int flags = SOCK_NONBLOCK | SOCK_CLOEXEC;

    socklen_t addr_len = peer_size;
    auto sa_ptr = static_cast<sockaddr*>( peer_sa );

    auto res = linux_call::no_err( ::accept4, fd, sa_ptr, &addr_len, flags );

    if ( addr_len > peer_size ) throw verror << "Not supported size of address";   // [1]

    return res;
}
//=======================================================================================
ssize_t wrap_sys_socket::recv_no_err( int fd, void *buf, size_t len, int flags )
{
    return linux_call::no_err( ::recv, fd, buf, len, flags );
}
//=======================================================================================
static ssize_t _send( int fd, const void* buf, size_t sz )
{
    return linux_call::no_err( ::send, fd, buf, sz, MSG_NOSIGNAL );
}
//=======================================================================================
//  Необходимо убедиться что все данные зохаваны в систему для передачи. Поэтому в цикле.
bool wrap_sys_socket::send_no_err( int fd, const std::string *data )
{
    auto ptr  = data->c_str();
    auto size = data->size();
    while (true)
    {
        auto sended = _send(fd, ptr, size);
        size_t usended = size_t( sended );

        if ( sended < 0 )      return false;
        if ( usended == size ) return true;

        assert( usended < size );
        ptr  += usended;
        size -= usended;
    }
}
//=======================================================================================
void wrap_sys_socket::keep_alive( int fd, int idle, int intvl, int cnt )
{
    set_sock_opt_int( fd, SOL_SOCKET, SO_KEEPALIVE, 1 );

    set_sock_opt_int( fd, IPPROTO_TCP, TCP_KEEPIDLE,  idle  );
    set_sock_opt_int( fd, IPPROTO_TCP, TCP_KEEPINTVL, intvl );
    set_sock_opt_int( fd, IPPROTO_TCP, TCP_KEEPCNT,   cnt   );
}
//=======================================================================================
