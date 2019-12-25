#ifndef VTCP_SOCKET_H
#define VTCP_SOCKET_H

#include <memory>
#include "vsignal.h"
#include "vsocket_address.h"

//=======================================================================================
class vtcp_socket final
{
public:
    class accepted_peer;

    enum class err_codes { no, connection_refused };

    using unique_ptr = std::unique_ptr<vtcp_socket>;
    using shared_ptr = std::shared_ptr<vtcp_socket>;

    static unique_ptr make_unique( accepted_peer * peer );
    static shared_ptr make_shared( accepted_peer * peer );

    vtcp_socket();
    vtcp_socket( accepted_peer *peer );
    ~vtcp_socket();

    vsignal<> connected;
    vsignal<> disconnected;
    vsignal<std::string> received;

    vsignal<> err_broken_pipe;      //  Обрыв соединения.
    vsignal<> err_conn_refused;     //  При подключении, когда нету доступа/ресурса.


    void connect( const vsocket_address& addr );
    bool is_connected() const;

    void close();

    void send( const std::string& data );

    void keep_alive( int idle, int intvl, int cnt );

    vsocket_address address() const;
    vsocket_address peer_address() const;

private:
    class _pimpl; std::unique_ptr<_pimpl> _p;
};
//=======================================================================================
class vtcp_socket::accepted_peer final
{
public:

    vtcp_socket::unique_ptr as_unique();
    vtcp_socket::shared_ptr as_shared();

    const vsocket_address& peer_address() const;

private:
    friend class vtcp_server; accepted_peer( int fd, vsocket_address && peer_addr );
    friend class vtcp_socket; int _take_fd();

    std::shared_ptr<int> _fd;
    vsocket_address _peer;
};
//=======================================================================================

#endif // VTCP_SOCKET_H
