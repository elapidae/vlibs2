#ifndef VSOCKET_ADDRESS_H
#define VSOCKET_ADDRESS_H

#include <string>

//=======================================================================================
class vsocket_address
{
public:
    static vsocket_address any_ip4( uint16_t port = 0 );
    static vsocket_address any_ip6( uint16_t port = 0 );
    static vsocket_address loopback_ip4( uint16_t port = 0 );
    static vsocket_address loopback_ip6( uint16_t port = 0 );

    vsocket_address();
    vsocket_address( const std::string& addr, uint16_t port );

    std::string address()   const;
    uint16_t    port()      const;
    std::string str()       const;

    bool is_valid() const;
    bool is_ip4()   const;
    bool is_ip6()   const;

private:
    static vsocket_address _ip4( uint32_t ip,     uint16_t port );
    static vsocket_address _ip6( const void *ip6, uint16_t port );

    static bool _init( const std::string& addr, uint16_t port , vsocket_address *dst );

    uint32_t _data[28/4]; // 28 is sizeof(sockaddr_in6)
    enum class _ip_type { Unknown, Ip4, Ip6 } _type = _ip_type::Unknown;

    friend class vtcp_socket;
    void* data()                            { return _data;         }
    const void* data() const                { return _data;         }
    unsigned data_size() const              { return sizeof(_data); }
};
//=======================================================================================


#endif // VSOCKET_ADDRESS_H
