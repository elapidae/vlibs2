#ifndef VTERMINAL_OPTIONS_H
#define VTERMINAL_OPTIONS_H

//=======================================================================================
/*  vterminal_options   2019-12-26  by elapidae
 *
 *  Развязка для структуры termios.
 *
 *  Промежуточный слой между нормальными людьми и настройкой порта.
 *
*/
//=======================================================================================

//=======================================================================================
class vterminal_options final
{
public:

    enum class parity    { no, odd, even          };
    enum class stop_bits { one, one_and_half, two };
    enum class flow_ctrl { no, hardware, software };

    explicit vterminal_options();
    explicit vterminal_options( int spd );
    explicit vterminal_options( int spd, parity, stop_bits, flow_ctrl );

    void set( int spd   );
    void set( parity    );
    void set( stop_bits );
    void set( flow_ctrl );

    void set( int spd, parity, stop_bits, flow_ctrl );

    //-----------------------------------------------------------------------------------
private:

    int _spd = 0;   //хранит уже в нужном коде (которые B****).

    parity    _par = parity::no;
    stop_bits _stb = stop_bits::one;
    flow_ctrl _flc = flow_ctrl::no;

    friend class vserial_port;
    void _store_as_binary( int fd ) const;
};
//=======================================================================================

#endif // VTERMINAL_OPTIONS_H
