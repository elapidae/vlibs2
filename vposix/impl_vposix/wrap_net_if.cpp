#include "impl_vposix/wrap_net_if.h"

#include <net/if.h>

#include "impl_vposix/linux_call.h"
#include "vcat.h"
//#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
unsigned int wrap_net_if::name_to_index( const std::string& ifname )
{
    // linux_call::no_err not actual, because function if_nametoindex is noexcept.
    auto res = ::if_nametoindex( ifname.c_str() );

    if ( res == 0 )
        ErrNo().do_throw( vcat("wrap_net_if::name_to_index(",ifname,").") );

    return res;
}
//=======================================================================================
std::string wrap_net_if::index_to_name( unsigned int ifindex )
{
    // linux_call::no_err not actual, because function if_indextoname is noexcept.
    char buf[IF_NAMESIZE];
    auto res = ::if_indextoname( ifindex, buf );

    if ( !res )
        ErrNo().do_throw( vcat("wrap_net_if::index_to_name(",ifindex,").") );

    return res;
}
//=======================================================================================
wrap_net_if::net_iface::vector wrap_net_if::interfaces()
{
    auto iface_ptr = ::if_nameindex();

    if ( !iface_ptr )
        ErrNo().do_throw( "wrap_net_if::interfaces()" );

    net_iface::vector res;
    for ( auto cur = iface_ptr;
          cur->if_index != 0 && cur->if_name != nullptr;
          ++cur
        )
    {
        res.push_back( {cur->if_index,cur->if_name} );
    }

    ::if_freenameindex( iface_ptr );

    return res;
}
//=======================================================================================
