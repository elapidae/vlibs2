#include "wrap_unistd.h"

#include <unistd.h>
#include "impl_vposix/linux_call.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
void wrap_unistd::close( int fd )
{
    linux_call::check( ::close, fd );
}
//=======================================================================================
void wrap_unistd::write( int fd, const std::string& data )
{
    ssize_t res = linux_call::no_err( ::write, fd, data.c_str(), data.size() );

    if ( res == ssize_t(data.size()) ) return;

    if ( res < 0 ) ErrNo().do_throw( "wrap_unistd::write" );
    throw verror << "Not all data was written";
}
//=======================================================================================
