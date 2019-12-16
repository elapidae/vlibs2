#ifndef VSOCKET_ADDRESS_H
#define VSOCKET_ADDRESS_H

#include <string>

//=======================================================================================
/*  2019-12-10      by elapidae
 *
 *  Это такой "промежуточный" класс, задача которого -- использовать его и как posix
 *  обертку со стороны v***_socket & v***_server и как нормальный адрес для юзверя.
 *
*/
//=======================================================================================

//=======================================================================================
class vsocket_address
{
public:
    static vsocket_address any_ip4( uint16_t port = 0 );
    static vsocket_address any_ip6( uint16_t port = 0 );
    static vsocket_address loopback_ip4( uint16_t port = 0 );
    static vsocket_address loopback_ip6( uint16_t port = 0 );

    vsocket_address();
    vsocket_address( const std::string& addr, uint16_t port = 0 );

    std::string ip()    const;
    uint16_t    port()  const;
    std::string str()   const;

    bool is_valid() const;
    bool is_ip4()   const;
    bool is_ip6()   const;

private:
    static vsocket_address _ip4( uint32_t ip,     uint16_t port );
    static vsocket_address _ip6( const void *ip6, uint16_t port );

    static bool _init( const std::string& addr, uint16_t port , vsocket_address *dst );

    uint32_t _raw_data[28/4]; // 28 is sizeof(sockaddr_in6)
    enum class _ip_type { Unknown, Ip4, Ip6 } _type = _ip_type::Unknown;

    friend class vtcp_socket;
    friend class vtcp_server;
    void* _data();
    const void* _data() const;
    unsigned _data_size() const;

    int _family() const;
};
//=======================================================================================
std::ostream& operator << ( std::ostream& os, const vsocket_address& addr );
//=======================================================================================


#endif // VSOCKET_ADDRESS_H
