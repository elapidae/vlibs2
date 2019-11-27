#include "wrap_sys_file.h"

#include <sys/file.h>
#include "impl_vposix/linux_call.h"
using namespace impl_vposix;


//=======================================================================================
void wrap_sys_file::lock_exclusive( int fd )
{
    flock(fd,8);
    linux_call::check( ::flock, fd, LOCK_EX|LOCK_NB );
}
//=======================================================================================
