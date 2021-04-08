#ifndef VSOCKET_ADDRESS_H
#define VSOCKET_ADDRESS_H

#include <string>

//=======================================================================================
/*  2019-12-10      by elapidae
 *
 *  Это такой "промежуточный" класс, задача которого -- использовать его и как posix
 *  обертку со стороны v***_socket & v***_server и как нормальный адрес для юзверя.
 *  Основная его задача -- не пущать наружу структуры sockaddr*, и прочую arpa дичь,
 *  т.е. чтобы у юзверя не торчало всёвотэто в пространстве имен.
 *
*/
//=======================================================================================

//=======================================================================================
class vsocket_address
{
public:
    static vsocket_address any_ip4( uint16_t port = 0 )         noexcept;
    static vsocket_address any_ip6( uint16_t port = 0 )         noexcept;
    static vsocket_address loopback_ip4( uint16_t port = 0 )    noexcept;
    static vsocket_address loopback_ip6( uint16_t port = 0 )    noexcept;

    static vsocket_address create( const std::string& addr,
                                   uint16_t port = 0,
                                   bool *ok = nullptr );

    //  About to use with ::getaddrinfo.
    static vsocket_address from_raw_sockaddr( const void* sockaddr, uint sockaddr_len,
                                              int type );

    static vsocket_address from_raw_ip4( uint32_t ip,       uint16_t port = 0 ) noexcept;
    static vsocket_address from_raw_ip6( const void* store, uint16_t port = 0 ) noexcept;

    vsocket_address() noexcept;
    vsocket_address( const std::string& addr, uint16_t port = 0 );

    std::string ip()    const;
    uint16_t    port()  const noexcept;
    std::string str()   const;

    bool is_valid() const noexcept;
    bool is_ip4()   const noexcept;
    bool is_ip6()   const noexcept;

    void set_port( uint16_t new_port );

    //-----------------------------------------------------------------------------------
private:
    void _init() noexcept;
    static bool _init( const std::string& addr, uint16_t port, vsocket_address *dst );

    static constexpr auto _raw_data_size = 28;
    uint32_t _raw_data[_raw_data_size/4]; // 28 is sizeof(sockaddr_in6)

    friend class vtcp_socket;
    friend class vtcp_server;
    friend class vudp_socket;
    void*       _data()                 noexcept;
    const void* _data()         const   noexcept;
    unsigned    _data_size()    const   noexcept;

    int _family() const noexcept;
};
//=======================================================================================
std::ostream& operator << ( std::ostream& os, const vsocket_address& addr );
//=======================================================================================


#endif // VSOCKET_ADDRESS_H
