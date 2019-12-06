#include "vsocket_address.h"

#include <string.h>
#include <arpa/inet.h>

#include "impl_vposix/wrap_arpa_inet.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
vsocket_address vsocket_address::_ip4( uint32_t ip, uint16_t port )
{
    vsocket_address res;
    bzero( res._data, sizeof(res._data) );

    void* vptr = static_cast<void*>( res._data );
    auto s4 = static_cast<sockaddr_in*>( vptr );

    s4->sin_family      = AF_INET;
    s4->sin_addr.s_addr = htonl( ip );          //  Мерзкие функции...
    s4->sin_port        = htons( port );        //

    res._type = _ip_type::Ip4;

    return res;
}
//=======================================================================================
vsocket_address vsocket_address::_ip6( const void *ip6, uint16_t port )
{
    vsocket_address res;
    void* vptr = static_cast<void*>( res._data );
    bzero( res._data, sizeof(res._data) );

    auto s6 = static_cast<sockaddr_in6*>( vptr );

    s6->sin6_family = AF_INET;
    s6->sin6_addr   = *static_cast<const in6_addr*>( ip6 );
    s6->sin6_port   = htons(port);

    res._type = _ip_type::Ip6;

    return res;
}
//=======================================================================================
vsocket_address vsocket_address::loopback_ip4(uint16_t port)
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
    static_assert( sizeof(_data)  == sizeof(sockaddr_in6),  "sockaddr_in6 size"  );
    static_assert( alignof(_data) == alignof(sockaddr_in6), "sockaddr_in6 align" );
}
#pragma GCC diagnostic pop
//=======================================================================================
vsocket_address::vsocket_address( const std::string& addr, uint16_t port )
{
    bool ok = _init( addr, port, this );

    if (!ok) throw verror << "Address '" << addr << "' is wrong.";
}
//=======================================================================================
std::string vsocket_address::address() const
{
    const void* vptr = static_cast<const void*>( _data );
    auto s4 = static_cast<const sockaddr_in*>( vptr );
    auto s6 = static_cast<const sockaddr_in6*>( vptr );

    if ( _type == _ip_type::Ip4 ) return wrap_arpa_inet::inet_ntop_ip4( &s4->sin_addr  );
    if ( _type == _ip_type::Ip6 ) return wrap_arpa_inet::inet_ntop_ip6( &s6->sin6_addr );

    return "NOT_VALID";
}
//=======================================================================================
uint16_t vsocket_address::port() const
{
    const void* vptr = static_cast<const void*>( _data );
    auto s4 = static_cast<const sockaddr_in*>( vptr );
    auto s6 = static_cast<const sockaddr_in6*>( vptr );

    if ( _type == _ip_type::Ip4 ) return s4->sin_port;
    if ( _type == _ip_type::Ip6 ) return s6->sin6_port;

    return 0;
}
//=======================================================================================
std::string vsocket_address::str() const
{
    return address() + ':' + std::to_string(port());
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
bool vsocket_address::_init(const std::string& addr, uint16_t port, vsocket_address *dst)
{
    bzero( dst->_data, sizeof(dst->_data) );

    void* vptr = static_cast<void*>( dst->_data );

    sockaddr_in *s4 = static_cast<sockaddr_in*>( vptr );
    bool ok = wrap_arpa_inet::inet_pton_ip4( addr, &s4->sin_addr );
    if (ok)
    {
        dst->_type = _ip_type::Ip4;

        s4->sin_port   = port;
        s4->sin_family = AF_INET;
        return true;
    }
    sockaddr_in6 *s6 = static_cast<sockaddr_in6*>( vptr );
    ok = wrap_arpa_inet::inet_pton_ip6( addr, &s6->sin6_addr );
    if (!ok) return false;

    dst->_type = _ip_type::Ip6;

    s6->sin6_port = port;
    s6->sin6_family = AF_INET6;
    return true;
}
//=======================================================================================
