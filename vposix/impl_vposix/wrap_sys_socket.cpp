#include "wrap_sys_socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <cassert>
#include <netinet/tcp.h>

#include "impl_vposix/linux_call.h"
#include "vcat.h"
#include "vlog.h"

using namespace impl_vposix;


//=======================================================================================
//  protocol == 0 --> default for domain & type.
int wrap_sys_socket::socket( int domain, int type, int protocol )
{
    type |= SOCK_NONBLOCK | SOCK_CLOEXEC;

    return linux_call::check( ::socket, domain, type, protocol );
}
//=======================================================================================
int wrap_sys_socket::socket_tcp( int af_type )
{
    return socket( af_type, SOCK_STREAM, 0 );
}
//=======================================================================================
int wrap_sys_socket::socket_udp( int af_type )
{
    return socket( af_type, SOCK_DGRAM, 0 );
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
//=======================================================================================
static void set_sock_opt_int( int fd, int level, int optname, int val )
{
    set_sock_opt( fd, level, optname, &val, sizeof(val) );
}
//=======================================================================================
void wrap_sys_socket::set_out_of_band_data( int fd, bool set )
{
    set_sock_opt_int( fd, SOL_SOCKET, SO_OOBINLINE, set ? 1 : 0 );
}
//=======================================================================================
void wrap_sys_socket::set_reuse_address( int fd )
{
    set_sock_opt_int( fd, SOL_SOCKET, SO_REUSEADDR, 1 );
}
//=======================================================================================
void wrap_sys_socket::set_broadcast( int fd )
{
    set_sock_opt_int( fd, SOL_SOCKET, SO_BROADCAST, 1 );
}
//=======================================================================================
static bool shutdown_no_err( int fd, int directions )
{
    return 0 == linux_call::no_err( ::shutdown, fd, directions );
}
//=======================================================================================
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
//=======================================================================================
static int get_sock_opt_int( int fd, int level, int optname )
{
    int res;
    socklen_t len = sizeof( res );
    linux_call::check( ::getsockopt, fd, level, optname, &res, &len );
    assert( len == sizeof(res) );
    return res;
}
//=======================================================================================
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
size_t wrap_sys_socket::pending_datagram_size( int fd )
{
    char c;
    auto res = linux_call::no_err( ::recv, fd, &c, 1, MSG_PEEK | MSG_TRUNC );
    if ( res >= 0 ) return size_t(res);

    ErrNo e;
    if ( !e.resource_unavailable_try_again() )
        e.do_throw( "pending_datagram_size" );

    return 0;
}
//=======================================================================================
void wrap_sys_socket::get_sockaddr( int fd, void *buf, unsigned buf_size )
{
    auto sa_ptr = static_cast<sockaddr*>( buf );
    socklen_t m_len = buf_size;
    linux_call::check( ::getsockname, fd, sa_ptr, &m_len );
    assert( m_len <= buf_size );
}
//=======================================================================================
void wrap_sys_socket::get_peeraddr( int fd, void *buf, unsigned buf_size )
{
    auto sa_ptr = static_cast<sockaddr*>( buf );
    socklen_t m_len = buf_size;
    linux_call::check( ::getpeername, fd, sa_ptr, &m_len );
    assert( m_len <= buf_size );
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
ssize_t wrap_sys_socket::recv_from_no_err( int fd, void *buf, size_t buf_size,
                                           void *addr, unsigned addr_size )
{
    socklen_t m_addr_size = addr_size;
    auto m_addr = static_cast<sockaddr*>( addr );

    auto res = linux_call::no_err( ::recvfrom, fd,
                                   buf, buf_size,
                                   MSG_NOSIGNAL,
                                   m_addr, &m_addr_size );

    assert( res < 0 || m_addr_size > 0 );

    return res;
}
//=======================================================================================
size_t wrap_sys_socket::recv_from( int fd, void *buf, size_t buf_size,
                                   void *addr, unsigned addr_size )
{
    auto res = recv_from_no_err( fd, buf, buf_size, addr, addr_size );
    if ( res < 0 )
        ErrNo().do_throw( vcat("wrap_sys_socket::recv_from(",fd,")") );

    return size_t( res );
}
//=======================================================================================
static ssize_t _send_no_err( int fd, const void* buf, size_t sz )
{
    return linux_call::no_err( ::send, fd, buf, sz, MSG_NOSIGNAL );
}
//=======================================================================================
//  Необходимо убедиться что все данные зохаваны в систему для передачи. Поэтому в цикле.
bool wrap_sys_socket::send_no_err( int fd, const std::string& data )
{
    auto ptr  = data.c_str();
    auto size = data.size();
    while (true)
    {
        auto sended = _send_no_err(fd, ptr, size);
        size_t usended = size_t( sended );

        if ( sended < 0 )
        {
            // TODO poll it
            if (errno == EAGAIN || errno == EWOULDBLOCK )
            {
                continue;
            }
            return false;
        }
        if ( usended == size ) return true;

        assert( usended < size );
        ptr  += usended;
        size -= usended;

        vtrace["wrap_sys_socket"] << "Sent " << sended <<  ", rest size " << size;
    }
}
//=======================================================================================
void wrap_sys_socket::send_raw(int fd, const void *data , size_t len)
{
    auto sended = _send_no_err( fd, data, len );
    if ( sended < 0 ) ErrNo().do_throw( vcat("socket::send_1(",fd,",",data) );
}
//=======================================================================================
void wrap_sys_socket::send_to( int fd, const std::string &data,
                               const void *peer_sa, unsigned peer_size )
{
    auto sa = static_cast<const sockaddr*>( peer_sa );
    auto res = linux_call::check( ::sendto, fd,
                                  data.c_str(), data.size(),
                                  MSG_NOSIGNAL,
                                  sa, peer_size );

    if ( size_t(res) != data.size() )
        throw verror( "sendto: not all data sent: size = ", data.size(),
                      ", but sent ", res, " bytes..." );
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
