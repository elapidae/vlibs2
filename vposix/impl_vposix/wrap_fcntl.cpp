#include "wrap_fcntl.h"

#include <fcntl.h>
#include "vlog.h"

#include "impl_vposix/linux_call.h"

using namespace impl_vposix;

//=======================================================================================
//  http://man7.org/linux/man-pages/man2/open.2.html
int wrap_fcntl::open( const std::string& fname, int flags, mode_t mode )
{
    auto res = linux_call::no_err( ::open, fname.c_str(), flags, mode );

    if (res < 0)
        throw verror.oct()( "::open(",fname,',',flags,',',mode,") ", ErrNo().text() );

    return res;
}
//=======================================================================================
int wrap_fcntl::open_append( const std::string& fname )
{
    mode_t mode = 0664;

    return open( fname, O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC, mode );
}
//=======================================================================================
//  http://man7.org/linux/man-pages/man2/fcntl.2.html
int wrap_fcntl::file_control_get( int fd )
{
    return linux_call::check( ::fcntl, fd, F_GETFD );
}
//---------------------------------------------------------------------------------------
void wrap_fcntl::file_control_set( int fd, int flags )
{
    linux_call::check( ::fcntl, fd, F_SETFD, flags );
}
//=======================================================================================
//void wrap_fcntl::get_print_lock( int fd )
//{
//    flock l{0,0,0,0,0};
//    linux_call::check( ::fcntl, fd, F_GETLK, &l );

//    vdeb << "len:" << l.l_len;
//    vdeb << "pid:" << l.l_pid;
//    vdeb << "typ:" << l.l_type;// F_RDLCK
//    vdeb << "str:" << l.l_start;
//    vdeb << "whc:" << l.l_whence;
//}
//=======================================================================================
//void wrap_fcntl::set_write_lock( int fd )
//{
//    flock l;
//    l.l_whence  = SEEK_SET;     // с начала файла.
//    l.l_start   = 0;            //
//    l.l_len     = 0;            // блокируем весь файл, 0 -- специальное значение.
//    l.l_type    = F_WRLCK;      //
//    l.l_pid     = 0;

//    linux_call::check( ::fcntl, fd, F_OFD_SETLK, &l );
//}
//=======================================================================================
