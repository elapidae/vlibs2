#ifndef VTCP_SERVER_H
#define VTCP_SERVER_H

#include "vsocket_address.h"
#include "vtcp_socket.h"
#include "vsignal.h"

//=======================================================================================
class vtcp_server final
{
    enum { _queue_connections = 64 };
public:

    vsignal<vtcp_socket::accepted_peer> accepted;

    vtcp_server();
    ~vtcp_server();

    void listen( const vsocket_address& addr );
    void listen_any_ip4      ( uint16_t port = 0 );
    void listen_any_ip6      ( uint16_t port = 0 );
    void listen_loopback_ip4 ( uint16_t port = 0 );
    void listen_loopback_ip6 ( uint16_t port = 0 );

    vsocket_address address() const;

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};
//=======================================================================================

#endif // VTCP_SERVER_H
