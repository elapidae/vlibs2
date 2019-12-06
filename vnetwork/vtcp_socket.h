#ifndef VTCP_SOCKET_H
#define VTCP_SOCKET_H

#include <memory>
#include "vsignal.h"
#include "vsocket_address.h"
#include "vinet_address.h"

//=======================================================================================
class vtcp_socket final
{
public:
    class accepted_socket;

    vtcp_socket();
    ~vtcp_socket();

    vsignal<> connected;
    vsignal<> disconnected;
    vsignal<const std::string&> received;
    vsignal<const std::string&> error_occured;

    void connect( const vsocket_address& addr );

    void listen( const vsocket_address& addr );
    void listen_any      ( uint16_t port );
    void listen_loopback ( uint16_t port );

    void close();

    //  Закрывают поток данных в соответствующие стороны.
    //  В случае ошибок, не бросают исключения.
    bool shutdown_read();
    bool shutdown_write();
    bool shutdown_rw();

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};
//=======================================================================================
class vtcp_socket::accepted_socket final
{
public:
    ~accepted_socket();

private:
    friend class vtcp_server; accepted_socket( int fd );
    friend class vtcp_socket; int take();

    std::shared_ptr<std::atomic<int>> _p;
};
//=======================================================================================

#endif // VTCP_SOCKET_H
