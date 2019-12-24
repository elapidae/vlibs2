#ifndef VUDP_SOCKET_H
#define VUDP_SOCKET_H

#include <memory>
#include "vsocket_address.h"
#include "vsignal.h"

//=======================================================================================
class vudp_socket final
{
public:
    struct packet
    {
        std::string     data;
        vsocket_address peer;
    };

    vsignal<packet> received;

    using unique_ptr = std::unique_ptr<vudp_socket>;
    using shared_ptr = std::shared_ptr<vudp_socket>;

    static unique_ptr make_unique( const vsocket_address& addr );
    static shared_ptr make_shared( const vsocket_address& addr );


    vudp_socket();
    vudp_socket( const vsocket_address& addr );
    ~vudp_socket();

    void bind( const vsocket_address& addr );
    void bind_any_ip4( uint16_t port = 0 );
    void bind_any_ip6( uint16_t port = 0 );
    void bind_loopback_ip4( uint16_t port = 0 );
    void bind_loopback_ip6( uint16_t port = 0 );

    void close();

    void send_to( const vsocket_address& addr, const std::string& data );

    vsocket_address address() const;
    bool is_binded() const;

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};
//=======================================================================================

#endif // VUDP_SOCKET_H
