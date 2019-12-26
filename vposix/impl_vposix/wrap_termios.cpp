#include "wrap_termios.h"

//  Here: ioctl -- awfull function what do everything.
#include <sys/ioctl.h>

#include <termios.h>

#include "impl_vposix/linux_call.h"

using namespace impl_vposix;

//=======================================================================================
void wrap_termios::set_tio_exclusive( int fd, bool excl )
{
    if ( excl )
    {
    #ifdef TIOCEXCL
        linux_call::check( ::ioctl, fd, TIOCEXCL );
    #endif
    }
    else
    {
    #ifdef TIOCNXCL
        linux_call::check( ::ioctl, fd, TIOCNXCL );
    #endif
    }
}
//=======================================================================================
void wrap_termios::flush( int fd )
{
    linux_call::check( ::tcflush, fd, TCIOFLUSH );
}
//=======================================================================================
