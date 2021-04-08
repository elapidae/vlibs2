#include "vsocket_address.h"

#include <string.h>
#include <arpa/inet.h>

#include "impl_vposix/wrap_arpa_inet.h"
#include "vlog.h"
#include "vnetwork.h"

using namespace impl_vposix;

//=======================================================================================
/*  2019-12-10      by elapidae
 *
 *  NB! аргументы с именами prt используется (вместо port) в случае, когда надо применять
 *  к нему методы ntohs, ntohs.
 *
*/
//=======================================================================================


//=======================================================================================
//  helper
template<typename Res, typename From>
Res cast_over_void( From *f ) noexcept
{
    void *v = static_cast<void*>( f );
    return static_cast<Res>( v );
}
//---------------------------------------------------------------------------------------
template<typename Res, typename From>
Res cast_over_void( const From *f ) noexcept
{
    const void *v = static_cast<const void*>( f );
    return static_cast<Res>( v );
}
//=======================================================================================


//=======================================================================================
//      Мерзяйные функции, пусть хоть читаются.
static uint16_t port_host_to_net( uint16_t p ) noexcept
{
    return htons( p );
}
static uint32_t addr_host_to_net( uint32_t a ) noexcept
{
    return htonl( a );
}
static uint16_t port_net_to_host( uint16_t p ) noexcept
{
    return ntohs( p );
}
static uint32_t addr_net_to_host( uint32_t a ) noexcept
{
    return ntohl( a );
}
//=======================================================================================


//=======================================================================================
vsocket_address vsocket_address::loopback_ip4( uint16_t port ) noexcept
{
    return from_raw_ip4( INADDR_LOOPBACK, port );
}
//---------------------------------------------------------------------------------------
vsocket_address vsocket_address::any_ip4( uint16_t port ) noexcept
{
    return from_raw_ip4( INADDR_ANY, port );
}
//---------------------------------------------------------------------------------------
vsocket_address vsocket_address::any_ip6( uint16_t port ) noexcept
{
    return from_raw_ip6( &in6addr_any, port );
}
//---------------------------------------------------------------------------------------
vsocket_address vsocket_address::loopback_ip6( uint16_t port ) noexcept
{
    return from_raw_ip6( &in6addr_loopback, port );
}
//=======================================================================================
vsocket_address vsocket_address::create(const std::string &addr, uint16_t port, bool *ok)
{
    vsocket_address res;

    auto m_ok = _init( addr, port, &res );
    if (ok) *ok = m_ok;

    return res;
}
//=======================================================================================
vsocket_address vsocket_address::from_raw_ip4( uint32_t ip, uint16_t port ) noexcept
{
    vsocket_address res;

    auto s4 = cast_over_void<sockaddr_in*>( res._data() );

    s4->sin_family      = AF_INET;
    s4->sin_addr.s_addr = addr_host_to_net( ip   );       //  Мерзкие функции...
    s4->sin_port        = port_host_to_net( port );       //

    return res;
}
//=======================================================================================
vsocket_address vsocket_address::from_raw_ip6( const void *store, uint16_t port) noexcept
{
    vsocket_address res;

    auto s6 = cast_over_void<sockaddr_in6*>( res._data() );

    s6->sin6_family = AF_INET6;
    s6->sin6_addr   = *cast_over_void<const in6_addr*>( store );
    s6->sin6_port   = port_host_to_net( port );

    return res;
}
//=======================================================================================
vsocket_address
vsocket_address::from_raw_sockaddr( const void *sockaddr, uint sockaddr_len, int type )
{
    if ( type == AF_INET )
    {
        if ( sockaddr_len != sizeof(sockaddr_in) )
            throw verror << "Bad size" << sockaddr_len;

        auto addr = static_cast<const sockaddr_in*>( sockaddr );
        return from_raw_ip4( addr_net_to_host(addr->sin_addr.s_addr),
                             port_net_to_host(addr->sin_port) );
    }

    if ( type == AF_INET6 )
    {
        if ( sockaddr_len != sizeof(sockaddr_in6) )
            throw verror << "Bad size" << sockaddr_len;

        auto addr = static_cast<const sockaddr_in6*>( sockaddr );
        return from_raw_ip6( addr->sin6_addr.s6_addr,
                             port_net_to_host(addr->sin6_port) );
    }

    throw verror << "Unknown type: " << type;
}
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wgnu-alignof-expression"
vsocket_address::vsocket_address() noexcept
{
    static_assert( sizeof(_raw_data)  == sizeof(sockaddr_in6),  "sockaddr_in6 size"  );
    static_assert( alignof(_raw_data) == alignof(sockaddr_in6), "sockaddr_in6 align" );

    _init();
}
#pragma GCC diagnostic pop
//=======================================================================================
vsocket_address::vsocket_address( const std::string& addr, uint16_t port )
{
    bool ok = _init( addr, port, this );
    if (!ok) throw verror << "Address '" << addr << "' is wrong.";
}
//=======================================================================================
std::string vsocket_address::ip() const
{
    auto s4 = cast_over_void<const sockaddr_in*> ( _data() );
    auto s6 = cast_over_void<const sockaddr_in6*>( _data() );

    if ( is_ip4() ) return wrap_arpa_inet::inet_ntop_ip4( &s4->sin_addr  );
    if ( is_ip6() ) return wrap_arpa_inet::inet_ntop_ip6( &s6->sin6_addr );

    return "NOT_VALID";
}
//=======================================================================================
uint16_t vsocket_address::port() const noexcept
{
    auto s4 = cast_over_void<const sockaddr_in*> ( _data() );
    auto s6 = cast_over_void<const sockaddr_in6*>( _data() );

    if ( is_ip4() ) return port_net_to_host( s4->sin_port  );
    if ( is_ip6() ) return port_net_to_host( s6->sin6_port );

    return 0;
}
//=======================================================================================
std::string vsocket_address::str() const
{
    return ip() + '[' + std::to_string(port()) + ']';
}
//=======================================================================================
bool vsocket_address::is_valid() const noexcept
{
    return is_ip4() || is_ip6();
}
//=======================================================================================
bool vsocket_address::is_ip4() const noexcept
{
    return _family() == AF_INET;
}
//=======================================================================================
bool vsocket_address::is_ip6() const noexcept
{
    return _family() == AF_INET6;
}
//=======================================================================================
void vsocket_address::set_port( uint16_t new_port )
{
    new_port = port_host_to_net( new_port );

    auto s4 = cast_over_void<sockaddr_in*> ( _data() );
    auto s6 = cast_over_void<sockaddr_in6*>( _data() );

    if ( is_ip4() ) s4->sin_port  = new_port;
    else
    if ( is_ip6() ) s6->sin6_port = new_port;
    else
    throw verror << "Cannot set port, socket address is invalid.";
}
//=======================================================================================
void vsocket_address::_init() noexcept
{
    bzero( _data(), _raw_data_size );
    auto addr = cast_over_void<sockaddr*>( _data() );
    addr->sa_family = AF_UNSPEC;
}
//---------------------------------------------------------------------------------------
bool vsocket_address::_init( const std::string& addr, uint16_t prt, vsocket_address *dst)
{
    dst->_init();

    sockaddr_in *s4 = cast_over_void<sockaddr_in*>( dst->_data() );
    bool ok = wrap_arpa_inet::inet_pton_ip4( addr, &s4->sin_addr );
    if (ok)
    {
        s4->sin_port   = port_host_to_net( prt );
        s4->sin_family = AF_INET;
        return true;
    }

    sockaddr_in6 *s6 = cast_over_void<sockaddr_in6*>( dst->_data() );
    ok = wrap_arpa_inet::inet_pton_ip6( addr, &s6->sin6_addr );
    if (ok)
    {
        s6->sin6_port   = port_host_to_net( prt );
        s6->sin6_family = AF_INET6;
        return true;
    }

    auto may_addrs = vnetwork::resolve_address( addr );
    for ( auto & may_a: may_addrs )
    {
        if ( may_a.protocol == vnetwork::type_of_protocol::tcp )
        {
            *dst = may_a.address;
            dst->set_port( prt );
            return true;
        }
    }
    return false;
}
//=======================================================================================
void *vsocket_address::_data() noexcept
{
    return _raw_data;
}
//---------------------------------------------------------------------------------------
const void *vsocket_address::_data() const noexcept
{
    return is_valid() ? _raw_data : nullptr;
}
//---------------------------------------------------------------------------------------
unsigned vsocket_address::_data_size() const noexcept
{
    return _raw_data_size;
}
//---------------------------------------------------------------------------------------
int vsocket_address::_family() const noexcept
{
    //  Do not use _data() here, will be recurse queries.
    auto addr = cast_over_void<const sockaddr*>( _raw_data );
    return addr->sa_family;
}
//=======================================================================================


//=======================================================================================
std::ostream &operator << ( std::ostream& os, const vsocket_address& addr )
{
    os << "vsocket_address(" << addr.str() << ')';
    return os;
}
//=======================================================================================
