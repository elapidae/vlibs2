#ifndef IMPL_VPOSIX_WRAP_net_if_H
#define IMPL_VPOSIX_WRAP_net_if_H

//=======================================================================================
/*      20-03-2020  by elapidae
 *
 * http://ru.manpages.org/if_nametoindex/3
 * http://man7.org/linux/man-pages/man3/if_nameindex.3.html
 *
 *  TODO:
 *  UPD 2020-04-09 -- методы из этого враппера следует перенести в vnetwork, тогда
 *  список интерфейсов можно реализовать без лишних сущностей в интерфейсе vnetwork.
*/
//=======================================================================================

#include <string>
#include <vector>

//=======================================================================================
namespace impl_vposix
{
    class wrap_net_if
    {
    public:
        static unsigned int name_to_index( const std::string& ifname );
        static std::string  index_to_name( unsigned int ifindex      );

        struct net_iface
        {
            using vector = std::vector<net_iface>;
            unsigned int index;
            std::string  name;
        };

        static net_iface::vector interfaces();
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_net_if_H
