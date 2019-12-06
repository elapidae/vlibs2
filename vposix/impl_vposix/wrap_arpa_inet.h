#ifndef IMPL_VPOSIX_WRAP_ARPA_INET_H
#define IMPL_VPOSIX_WRAP_ARPA_INET_H

//=======================================================================================
/*
 *
 * http://man7.org/linux/man-pages/man3/inet_pton.3.html
 * http://man7.org/linux/man-pages/man3/inet_ntop.3.html
*/
//=======================================================================================

#include <string>

namespace impl_vposix
{
    struct wrap_arpa_inet
    {
        //  dst is written in network byte order.
        static bool inet_pton_ip4( const std::string& src, void *dst );
        static bool inet_pton_ip6( const std::string& src, void *dst );

        static std::string inet_ntop_ip4( const void *src );
        static std::string inet_ntop_ip6( const void *src );
    };
} // namespace impl_vposix

#endif // IMPL_VPOSIX_WRAP_ARPA_INET_H
