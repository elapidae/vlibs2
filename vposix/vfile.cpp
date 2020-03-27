#include "vfile.h"

#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/wrap_fcntl.h"
#include "vlog.h"

#include <fcntl.h> // need for flags O_*...

using namespace impl_vposix;

//=======================================================================================
bool vfile::unlink_no_err( const std::string& filename )
{
    return wrap_unistd::unlink_no_err( filename );
}
//=======================================================================================
vfile::~vfile()
{
    close();
}
//=======================================================================================
void vfile::close()
{
    if ( _fd >= 0 )
        wrap_unistd::close( _fd );
}
//=======================================================================================
std::string vfile::filename() const
{
    return _filename;
}
//=======================================================================================

//=======================================================================================
vfile_out::vfile_out( const std::string& fname, method how )
{
    int flags = O_WRONLY | O_LARGEFILE | O_CREAT | FD_CLOEXEC;

    if      ( how == truncate ) flags |= O_TRUNC;
    else if ( how == append   ) flags |= O_APPEND;

    mode_t mode = 0664;
    _fd = wrap_fcntl::open( fname, flags, mode );

    _filename = fname;
}
//=======================================================================================
void vfile_out::write( const std::string& data )
{
    if ( _fd < 0 )
        throw verror << "File '" << filename() << "' is not open.";

    wrap_unistd::write( _fd, data );
}
//=======================================================================================
