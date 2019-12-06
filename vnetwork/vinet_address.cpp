#include "vinet_address.h"

#include <arpa/inet.h> // need for checking

#include "impl_vposix/wrap_arpa_inet.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wgnu-alignof-expression"
vinet_address::vinet_address()
{
    static_assert( sizeof(_data)  == sizeof(in6_addr),  "bad data size"  );
    static_assert( alignof(_data) == alignof(in6_addr), "bad data align" );
}
#pragma GCC diagnostic pop
//=======================================================================================
void vinet_address::_from_string(const std::string &addr, vinet_address *dst)
{
    dst->_type = Type::Unknown;

    bool ok = wrap_arpa_inet::inet_pton_ip4( addr, dst->_data );
    if ( ok )
    {
        dst->_type = Type::Ip4;
        return;
    }

    ok = wrap_arpa_inet::inet_pton_ip6( addr, dst->_data );
    if ( ok )
        dst->_type = Type::Ip6;
}
//=======================================================================================
vinet_address vinet_address::from_string( const std::string& addr )
{
    vinet_address res;
    _from_string( addr, &res );
    return res;
}
//=======================================================================================
vinet_address::vinet_address( const std::string& addr )
{
    _from_string( addr, this );

    if ( !is_valid() )
        throw verror << "Address '" << addr << "' is wrong.";
}
//=======================================================================================
std::string vinet_address::str() const
{
    if ( _type == Type::Ip4 ) return wrap_arpa_inet::inet_ntop_ip4( _data );
    if ( _type == Type::Ip6 ) return wrap_arpa_inet::inet_ntop_ip6( _data );

    return "NOT_VALID";
}
//=======================================================================================
bool vinet_address::is_valid() const
{
    return _type != Type::Unknown;
}
//=======================================================================================
bool vinet_address::is_ip4() const
{
    return _type == Type::Ip4;
}
//=======================================================================================
bool vinet_address::is_ip6() const
{
    return _type == Type::Ip6;
}
//=======================================================================================
