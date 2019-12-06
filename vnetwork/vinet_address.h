#ifndef VINET_ADDRESS_H
#define VINET_ADDRESS_H

#include <string>

//=======================================================================================
class vinet_address
{
public:
    enum class Type { Unknown, Ip4, Ip6 };

    //  Не бросает исключения, вместо этого вернет невалидный vinet_address.
    static vinet_address from_string( const std::string& addr );

    // Будет невалидный.
    explicit vinet_address();

    // Если адрес не расшифруется, бросит исключение.
    explicit vinet_address( const std::string& addr );

    //  Если невалидный, возвращает "NOT_VALID".
    std::string str() const;

    bool is_valid() const;
    bool is_ip4()   const;
    bool is_ip6()   const;


private:
    static void _from_string( const std::string& addr, vinet_address *dst );

    // friends has access to _data
    uint32_t _data[4];

    Type _type = Type::Unknown;
};
//=======================================================================================

#endif // VINET_ADDRESS_H
