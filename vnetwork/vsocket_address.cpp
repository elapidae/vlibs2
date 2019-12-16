#include "vsocket_address.h"

#include <string.h>
#include <arpa/inet.h>

#include "impl_vposix/wrap_arpa_inet.h"
#include "vlog.h"

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
Res cast_over_void( From *f )
{
    void *v = static_cast<void*>( f );
    return static_cast<Res>( v );
}
template<typename Res, typename From>
Res cast_over_void( const From *f )
{
    const void *v = static_cast<const void*>( f );
    return static_cast<Res>( v );
}
//=======================================================================================


//=======================================================================================
//      Мерзяйные функции, пусть хоть читаются.
static uint16_t to_sys_port( uint16_t p )
{
    return htons( p );
}
static uint32_t to_sys_addr( uint32_t a )
{
    return htonl( a );
}
static uint16_t from_sys_port( uint16_t p )
{
    return ntohs( p );
}
//  Unused while, may be not need.
//static uint32_t from_sys_addr( uint32_t a )
//{
//    return ntohl( a );
//}
//=======================================================================================


//=======================================================================================
vsocket_address vsocket_address::_ip4( uint32_t ip, uint16_t prt )
{
    vsocket_address res;    

    auto s4 = cast_over_void<sockaddr_in*>( res._raw_data );

    s4->sin_family      = AF_INET;
    s4->sin_addr.s_addr = to_sys_addr( ip  );        //  Мерзкие функции...
    s4->sin_port        = to_sys_port( prt );        //

    res._type = _ip_type::Ip4;

    return res;
}
//=======================================================================================
vsocket_address vsocket_address::_ip6( const void *ip6, uint16_t prt )
{
    vsocket_address res;

    auto s6 = cast_over_void<sockaddr_in6*>( res._raw_data );

    s6->sin6_family = AF_INET6;
    s6->sin6_addr   = *cast_over_void<const in6_addr*>( ip6 );
    s6->sin6_port   = to_sys_port( prt );

    res._type = _ip_type::Ip6;

    return res;
}
//=======================================================================================
vsocket_address vsocket_address::loopback_ip4( uint16_t port )
{
    return _ip4( INADDR_LOOPBACK, port );
}
//---------------------------------------------------------------------------------------
vsocket_address vsocket_address::any_ip4( uint16_t port )
{
    return _ip4( INADDR_ANY, port );
}
//---------------------------------------------------------------------------------------
vsocket_address vsocket_address::any_ip6( uint16_t port )
{
    return _ip6( &in6addr_any, port );
}
//---------------------------------------------------------------------------------------
vsocket_address vsocket_address::loopback_ip6( uint16_t port )
{
    return _ip6( &in6addr_loopback, port );
}
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wgnu-alignof-expression"
vsocket_address::vsocket_address()
{
    static_assert( sizeof(_raw_data)  == sizeof(sockaddr_in6),  "sockaddr_in6 size"  );
    static_assert( alignof(_raw_data) == alignof(sockaddr_in6), "sockaddr_in6 align" );

    bzero( _raw_data, sizeof(_raw_data) );
}
#pragma GCC diagnostic pop
//=======================================================================================
vsocket_address::vsocket_address( const std::string& addr, uint16_t port )
{
    bzero( _raw_data, sizeof(_raw_data) );
    bool ok = _init( addr, port, this );

    if (!ok) throw verror << "Address '" << addr << "' is wrong.";
}
//=======================================================================================
std::string vsocket_address::ip() const
{
    auto s4 = cast_over_void<const sockaddr_in*> ( _raw_data );
    auto s6 = cast_over_void<const sockaddr_in6*>( _raw_data );

    if ( is_ip4() ) return wrap_arpa_inet::inet_ntop_ip4( &s4->sin_addr  );
    if ( is_ip6() ) return wrap_arpa_inet::inet_ntop_ip6( &s6->sin6_addr );

    return "NOT_VALID";
}
//=======================================================================================
uint16_t vsocket_address::port() const
{
    auto s4 = cast_over_void<const sockaddr_in*> ( _raw_data );
    auto s6 = cast_over_void<const sockaddr_in6*>( _raw_data );

    if ( is_ip4() ) return from_sys_port( s4->sin_port  );
    if ( is_ip6() ) return from_sys_port( s6->sin6_port );

    return 0;
}
//=======================================================================================
std::string vsocket_address::str() const
{
    return ip() + '[' + std::to_string(port()) + ']';
}
//=======================================================================================
bool vsocket_address::is_valid() const
{
    return _type != _ip_type::Unknown;
}
//=======================================================================================
bool vsocket_address::is_ip4() const
{
    return _type == _ip_type::Ip4;
}
//=======================================================================================
bool vsocket_address::is_ip6() const
{
    return _type == _ip_type::Ip6;
}
//=======================================================================================
bool vsocket_address::_init(const std::string& addr, uint16_t prt, vsocket_address *dst)
{
//    void* vptr = static_cast<void*>( dst->_raw_data );

    sockaddr_in *s4 = cast_over_void<sockaddr_in*>( dst->_raw_data );
    bool ok = wrap_arpa_inet::inet_pton_ip4( addr, &s4->sin_addr );
    if (ok)
    {
        dst->_type = _ip_type::Ip4;

        s4->sin_port   = to_sys_port( prt );
        s4->sin_family = AF_INET;
        return true;
    }

    sockaddr_in6 *s6 = cast_over_void<sockaddr_in6*>( dst->_raw_data );
    ok = wrap_arpa_inet::inet_pton_ip6( addr, &s6->sin6_addr );
    if (!ok) return false;

    dst->_type = _ip_type::Ip6;

    s6->sin6_port   = to_sys_port( prt );
    s6->sin6_family = AF_INET6;
    return true;
}
//=======================================================================================
void *vsocket_address::_data()
{
//    if ( !is_valid() )
//        throw verror << "Bad type of socket address.";
    return _raw_data;
}
//---------------------------------------------------------------------------------------
const void *vsocket_address::_data() const
{
    if ( !is_valid() ) throw verror << "Bad type of socket address.";
    return _raw_data;
}
//---------------------------------------------------------------------------------------
unsigned vsocket_address::_data_size() const
{
    return sizeof( _raw_data );
}
//---------------------------------------------------------------------------------------
int vsocket_address::_family() const
{
    if ( is_ip4() ) return AF_INET;
    if ( is_ip6() ) return AF_INET6;
    throw verror << "Bad type of socket address.";
}
//=======================================================================================


//=======================================================================================
std::ostream &operator << ( std::ostream& os, const vsocket_address& addr )
{
    os << "vsocket_address(" << addr.str() << ')';
    return os;
}
//=======================================================================================
