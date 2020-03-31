#include "vnetwork.h"

#include <netdb.h>
#include "vlog.h"
#include "vcat.h"

//=======================================================================================
/*
 * http://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/
//=======================================================================================

template <class> class TD;

//=======================================================================================
static vnetwork::type_of_protocol protocol( int proto )
{
    if ( proto == IPPROTO_TCP ) return vnetwork::type_of_protocol::tcp;
    if ( proto == IPPROTO_UDP ) return vnetwork::type_of_protocol::udp;
    if ( proto == IPPROTO_IP  ) return vnetwork::type_of_protocol::ip;
    throw verror << "Unknown protocol type: " << proto;
}
//---------------------------------------------------------------------------------------
static const char * protocol( vnetwork::type_of_protocol proto )
{
    if ( proto == vnetwork::type_of_protocol::tcp ) return "TCP";
    if ( proto == vnetwork::type_of_protocol::udp ) return "UDP";
    if ( proto == vnetwork::type_of_protocol::ip  ) return "IP";
    throw verror << "Unknown protocol type: " << int(proto);
}
//=======================================================================================
static vnetwork::type_of_socket socket_type( int sock )
{
    if ( sock == SOCK_STREAM ) return vnetwork::type_of_socket::stream;
    if ( sock == SOCK_DGRAM  ) return vnetwork::type_of_socket::datagram;
    if ( sock == SOCK_RAW    ) return vnetwork::type_of_socket::raw;
    throw verror << "Unknown socket type: " << sock;
}
//---------------------------------------------------------------------------------------
static const char * socket_type( vnetwork::type_of_socket sock )
{
    if ( sock == vnetwork::type_of_socket::stream   ) return "STREAM";
    if ( sock == vnetwork::type_of_socket::datagram ) return "DATAGRAM";
    if ( sock == vnetwork::type_of_socket::raw      ) return "RAW";
    throw verror << "Unknown socket type: " << int(sock);
}
//=======================================================================================

//=======================================================================================
vnetwork::error::error( const std::string& msg )
    : std::runtime_error( msg )
{}
//=======================================================================================

//=======================================================================================
vnetwork::address_info::vector
vnetwork::resolve_address( const std::string& addr_, const std::string& serv_ )
{
    auto addr_ptr = addr_.empty() ? nullptr : addr_.c_str();
    auto serv_ptr = serv_.empty() ? nullptr : serv_.c_str();

    struct addrinfo *addr = nullptr;

    auto call_res = getaddrinfo( addr_ptr, serv_ptr, nullptr, &addr );
    if ( call_res != 0 )
        throw error( vcat("Error during resolve address '", addr_,
                          "', service '", serv_, "': ", gai_strerror(call_res)) );

    address_info::vector res;

    auto cur_ptr = addr;
    while ( cur_ptr )
    {
        try
        {
            address_info cur;
            cur.address = vsocket_address::from_raw_sockaddr( cur_ptr->ai_addr,
                                                              cur_ptr->ai_addrlen,
                                                              cur_ptr->ai_family );

            if (cur_ptr->ai_canonname)
                cur.canonical_name = cur_ptr->ai_canonname;

            cur.protocol    = protocol   ( cur_ptr->ai_protocol );
            cur.socket_type = socket_type( cur_ptr->ai_socktype );

            res.push_back( cur );

            cur_ptr = cur_ptr->ai_next;
        }
        catch (...)
        {
            freeaddrinfo( addr );
            throw;
        }
    } // while cur_ptr not null.
    freeaddrinfo( addr );

    return res;
}
//=======================================================================================
std::ostream &operator << ( std::ostream& os, const vnetwork::address_info& ai )
{
    os << "Addr(" << ai.address << ","
       << protocol(ai.protocol) << ","
       << socket_type(ai.socket_type);

    if ( !ai.canonical_name.empty() )
        os << "," << ai.canonical_name;

    os << ")";

    return os;
}
//=======================================================================================

