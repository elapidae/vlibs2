#include "wrap_arpa_inet.h"

#include <arpa/inet.h>
#include <cassert>
#include "impl_vposix/linux_call.h"

using namespace impl_vposix;


//=======================================================================================
bool wrap_arpa_inet::inet_pton( int family, const std::string &src, void *dst )
{
    auto res = linux_call::no_err( ::inet_pton, family, src.c_str(), dst );

    if (res == -1)
        ErrNo().do_throw( "wrap_arpa_inet::inet_pton_ip4" );

    return res == 1;    // if 0 -- not correct src
}
//---------------------------------------------------------------------------------------
bool wrap_arpa_inet::inet_pton_ip4( const std::string& src, void *dst )
{
    return inet_pton( AF_INET, src, dst );
}
//---------------------------------------------------------------------------------------
bool wrap_arpa_inet::inet_pton_ip6( const std::string& src, void *dst )
{
    return inet_pton( AF_INET6, src, dst );
}
//=======================================================================================

//=======================================================================================
//  [1] returns pointer, cannot linux_call.
static std::string _inet_ntop( int af, const void *src )
{
    static constexpr auto addr_len = INET6_ADDRSTRLEN + 1;

    char buf[ addr_len ];
    auto cres = ::inet_ntop( af, src, buf, addr_len ); // [1]
    assert( cres );
    return cres;
}
//---------------------------------------------------------------------------------------
std::string wrap_arpa_inet::inet_ntop_ip4( const void *src )
{
    return _inet_ntop( AF_INET, src );
}
//---------------------------------------------------------------------------------------
std::string wrap_arpa_inet::inet_ntop_ip6( const void *src )
{
    return _inet_ntop( AF_INET6, src );
}
//=======================================================================================
