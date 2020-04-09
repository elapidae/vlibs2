#ifndef VCAN_SOCKET_H
#define VCAN_SOCKET_H

#include <memory>
#include <vsignal.h>
#include <ostream>

//=======================================================================================
class vcan_socket final
{
public:
    using id_type = uint32_t;

    struct message final
    {
        id_type     id;
        std::string data;
        int         iface;
        id_type     rx;
        id_type     tx;
    };

    vsignal<message> received;

    void bind( const std::string& iface = {} ); // If empty -- listen all
    void connect(); // For BCM, not implemented.

    void send( id_type id, const std::string& data );

    vcan_socket();
    ~vcan_socket();

private:
    class _pimpl; _pimpl * _p;
};
//=======================================================================================
std::ostream& operator << ( std::ostream& os, const vcan_socket::message& msg );
//=======================================================================================


#endif // VCAN_SOCKET_H
