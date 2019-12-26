#include "vterminal_options.h"

//=======================================================================================
/*
 * http://www.opennet.ru/man.shtml?topic=tcgetattr&category=3&russian=0
 *
 * Есть такая фраза:
 *  Обратите внимание, что tcsetattr() сообщает об успешном завершении, если хотя бы одно
 *  из запрошенных изменений может быть успешно выполнено. Поэтому, при необходимости
 *  одновременного изменения нескольких параметров, может понадобится после этой функции
 *  вызвать tcgetattr() для того, чтобы убедиться, что все изменения были выполнены
 *  успешно.
 *
 * Так вот -- такая проверка не делается, все равно непонятно что с ней делать.
*/
//=======================================================================================


#include <termios.h>
#include <string.h> // bzero

#include "impl_vposix/linux_call.h"
#include "vlog.h"

using namespace impl_vposix;

//  Placed after class code.
static int to_Bspeed( int speed );
static void tio_set_parity( vterminal_options::parity p, termios *tio );
static void tio_set_stopbits( vterminal_options::stop_bits sb, termios *tio );
static void tio_set_flowcontrol( vterminal_options::flow_ctrl fc, termios *tio);


//=======================================================================================
vterminal_options::vterminal_options()
{}
//=======================================================================================
vterminal_options::vterminal_options( int spd )
    : _spd( spd )
{}
//=======================================================================================
vterminal_options::vterminal_options( int spd, parity par, stop_bits stb, flow_ctrl flc )
{
    set( spd, par, stb, flc );
}
//=======================================================================================
void vterminal_options::set( int spd )
{
    _spd = to_Bspeed( spd );
}
//=======================================================================================
void vterminal_options::set( parity par )
{
    _par = par;
}
//=======================================================================================
void vterminal_options::set( stop_bits stb )
{
    _stb = stb;
}
//=======================================================================================
void vterminal_options::set( flow_ctrl flc )
{
    _flc = flc;
}
//=======================================================================================
void vterminal_options::set( int spd, parity par, stop_bits stb, flow_ctrl flc )
{
    set( spd );
    set( par );
    set( stb );
    set( flc );
}
//=======================================================================================
void vterminal_options::_store_as_binary( int fd ) const
{
    termios tio;
    ::bzero( &tio, sizeof(tio) );
    linux_call::check( ::tcgetattr, fd, &tio );

    // set_common_props
    ::cfmakeraw( &tio );
    tio.c_cflag |= CLOCAL;
    tio.c_cc[VTIME] = 0;
    tio.c_cc[VMIN] = 0;
    tio.c_cflag |= CREAD;

    //  set bits 8
    tio.c_cflag &= decltype(tio.c_cflag)(~CSIZE);
    tio.c_cflag |= CS8;

    //  set all user options
    tio_set_parity( _par, &tio );
    tio_set_stopbits( _stb, &tio );
    tio_set_flowcontrol( _flc, &tio );

    linux_call::check( ::cfsetspeed, &tio, _spd );

    //  And cast it to port.
    linux_call::check( ::tcsetattr, fd, TCSANOW, &tio );
}
//=======================================================================================


//=======================================================================================
static int to_Bspeed( int speed )
{
    switch( speed )
    {
    case       0: return       B0;
    case      50: return      B50;
    case      75: return      B75;
    case     110: return     B110;
    case     134: return     B134;
    case     150: return     B150;
    case     200: return     B200;
    case     300: return     B300;
    case     600: return     B600;
    case    1200: return    B1200;
    case    1800: return    B1800;
    case    2400: return    B2400;
    case    4800: return    B4800;
    case    9600: return    B9600;
    case   19200: return   B19200;
    case   38400: return   B38400;
    case   57600: return   B57600;
    case  115200: return  B115200;
    case  230400: return  B230400;
    case  460800: return  B460800;
    case  500000: return  B500000;
    case  576000: return  B576000;
    case  921600: return  B921600;
    case 1000000: return B1000000;
    case 1152000: return B1152000;
    case 1500000: return B1500000;
    case 2000000: return B2000000;
    case 2500000: return B2500000;
    case 3000000: return B3000000;
    case 3500000: return B3500000;
    case 4000000: return B4000000;
    }
    throw verror( "Bad serial speed: ", speed );
}
//=======================================================================================
static void tio_set_parity( vterminal_options::parity p, termios *tio )
{
    tio->c_iflag &= decltype(tio->c_iflag)(~(PARMRK | INPCK));
    tio->c_iflag |= IGNPAR;

    switch (p)
    {
    case vterminal_options::parity::no:
        tio->c_cflag &= decltype(tio->c_cflag)(~PARENB);
        return;

    case vterminal_options::parity::even:
        tio->c_cflag &= decltype(tio->c_cflag)(~PARODD);
        tio->c_cflag |= PARENB;
        return;

    case vterminal_options::parity::odd:
        tio->c_cflag |= PARENB | PARODD;
        return;
    }
    throw verror("Bad parity");
}
//=======================================================================================
static void tio_set_stopbits( vterminal_options::stop_bits sb, termios *tio )
{
    switch ( sb )
    {
    case vterminal_options::stop_bits::one:
        tio->c_cflag &= decltype(tio->c_cflag)(~CSTOPB);
        return;
    case vterminal_options::stop_bits::two:
        tio->c_cflag |= CSTOPB;
        return;
    case vterminal_options::stop_bits::one_and_half:
        tio->c_cflag &= decltype(tio->c_cflag)(~CSTOPB);
        return;
    }
    throw verror("Bad stop bits");
}
//=======================================================================================
static void tio_set_flowcontrol( vterminal_options::flow_ctrl fc, termios *tio)
{
    switch ( fc )
    {
    case vterminal_options::flow_ctrl::no:
        tio->c_cflag &= ~CRTSCTS;
        tio->c_iflag &= decltype(tio->c_iflag)(~(IXON | IXOFF | IXANY));
        return;
    case vterminal_options::flow_ctrl::hardware:
        tio->c_cflag |= CRTSCTS;
        tio->c_iflag &= decltype(tio->c_iflag)(~(IXON | IXOFF | IXANY));
        return;
    case vterminal_options::flow_ctrl::software:
        tio->c_cflag &= ~CRTSCTS;
        tio->c_iflag |= IXON | IXOFF | IXANY;
        return;
    }
    throw verror("Bad flow control");
}
//=======================================================================================
