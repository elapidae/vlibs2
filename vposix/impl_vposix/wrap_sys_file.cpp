#include "wrap_sys_file.h"

#include <sys/file.h>
#include "impl_vposix/linux_call.h"
using namespace impl_vposix;


//=======================================================================================
bool wrap_sys_file::try_lock_exclusive( int fd )
{
    auto res = linux_call::no_err( ::flock, fd, LOCK_EX|LOCK_NB );
    if ( res == 0 ) return true;

    ErrNo err;
    if ( err.resource_unavailable_try_again() )
        return false;

    throw posix_error( "wrap_sys_file::try_lock_exclusive( int fd )" );
}
//=======================================================================================
