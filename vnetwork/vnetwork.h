#ifndef VNETWORK_H
#define VNETWORK_H

#include <ostream>
#include <vector>
#include <stdexcept>
#include "vsocket_address.h"

//=======================================================================================
class vnetwork
{
public:
    //-----------------------------------------------------------------------------------

    class error : public std::runtime_error
    {
    public:
        error( const std::string& msg );
    };

    //-----------------------------------------------------------------------------------

    enum class type_of_protocol    { tcp, udp, ip };
    enum class type_of_socket      { stream, datagram, raw };

    //-----------------------------------------------------------------------------------

    struct address_info final
    {
        using vector = std::vector<address_info>;

        vsocket_address     address;

        type_of_protocol    protocol;
        type_of_socket      socket_type;

        std::string         canonical_name;
    };

    //-----------------------------------------------------------------------------------
    //  Address or service may be empty, service is such as 'http', 'imap', etc.
    //  See file /etc/services.
    static address_info::vector
    resolve_address( const std::string& addr, const std::string& service = {} );

    //-----------------------------------------------------------------------------------
};
//=======================================================================================
std::ostream& operator << (std::ostream& os, const vnetwork::address_info& ai );
//=======================================================================================


#endif // VNETWORK_H
