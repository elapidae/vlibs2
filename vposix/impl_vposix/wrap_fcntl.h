#ifndef WRAP_FCNTL_H
#define WRAP_FCNTL_H

//=======================================================================================
/*
 * http://man7.org/linux/man-pages/man0/fcntl.h.0p.html
 *
 *
 * The following shall be declared as functions and may also be defined as macros.
 * Function prototypes shall be provided.
 *
 *    int  creat(const char *, mode_t);
 *    int  fcntl(int, int, ...);
 *    int  open(const char *, int, ...);
 *    int  openat(int, const char *, int, ...);
 *    int  posix_fadvise(int, off_t, off_t, int);
 *    int  posix_fallocate(int, off_t, off_t);
*/
//=======================================================================================

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_fcntl final
    {
        //-------------------------------------------------------------------------------

        //  returns file descriptor
        static int open( const std::string& fname, int flags, mode_t mode );

        //  flags = O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC, mode = 0664;
        static int open_append( const std::string& fname );

        //-------------------------------------------------------------------------------

        static int  file_control_get( int fd );
        static void file_control_set( int fd, int flags );

        //-------------------------------------------------------------------------------
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // WRAP_FCNTL_H
