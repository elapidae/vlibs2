#ifndef WRAP_FCNTL_H
#define WRAP_FCNTL_H

//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man0/fcntl.h.0p.html
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
        static int open( const std::string& fname, int flags );

        //  flags = O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC, mode = 0664;
        static int open_append( const std::string& fname );

        //-------------------------------------------------------------------------------

        static int  get_file_control( int fd );
        static void set_file_control( int fd, int flags );

        static int  get_status_control( int fd );
        static void set_status_control( int fd, int flags );

        static bool get_nonblock( int fd );
        static void set_nonblock( int fd, bool nonblock );

        //-------------------------------------------------------------------------------

        //  Хотелось уметь блокировать файлы, но не взлетело. Надо RTFM.
        static void get_print_lock( int fd );
        static void set_write_lock( int fd );

        //-------------------------------------------------------------------------------
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // WRAP_FCNTL_H
