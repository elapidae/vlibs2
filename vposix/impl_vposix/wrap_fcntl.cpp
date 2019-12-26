#include "wrap_fcntl.h"

#include <exception>
#include <fcntl.h>
#include "vlog.h"
#include "vcat.h"

#include "impl_vposix/linux_call.h"

using namespace impl_vposix;

//=======================================================================================
//  http://man7.org/linux/man-pages/man2/open.2.html
int wrap_fcntl::open( const std::string& fname, int flags, mode_t mode )
{
    auto res = linux_call::no_err( ::open, fname.c_str(), flags, mode );

    if (res >= 0) return res;

    auto text = vcat().oct()("::open(",fname,',',flags,',',mode,") ",
                             ErrNo().text() ).str();

    throw std::ios_base::failure( text );
}
//=======================================================================================
int wrap_fcntl::open( const std::string& fname, int flags )
{
    auto res = linux_call::no_err( ::open, fname.c_str(), flags );

    if (res >= 0) return res;

    auto text = vcat().oct()( "::open(",fname,',',flags,") ", ErrNo().text() ).str();
    throw std::ios_base::failure( text );
}
//=======================================================================================
int wrap_fcntl::open_append( const std::string& fname )
{
    mode_t mode = 0664;

    return open( fname, O_WRONLY|O_CREAT|O_APPEND|O_CLOEXEC, mode );
}
//=======================================================================================
//  http://man7.org/linux/man-pages/man2/fcntl.2.html
int wrap_fcntl::get_file_control( int fd )
{
    return linux_call::check( ::fcntl, fd, F_GETFD );
}
//---------------------------------------------------------------------------------------
void wrap_fcntl::set_file_control( int fd, int flags )
{
    linux_call::check( ::fcntl, fd, F_SETFD, flags );
}
//---------------------------------------------------------------------------------------
int wrap_fcntl::get_status_control( int fd )
{
    return linux_call::check( ::fcntl, fd, F_GETFL );
}
//---------------------------------------------------------------------------------------
void wrap_fcntl::set_status_control(int fd, int flags)
{
    linux_call::check( ::fcntl, fd, F_SETFL, flags );
}
//=======================================================================================
bool wrap_fcntl::get_nonblock( int fd )
{
    auto flags = get_status_control( fd );
    return flags & O_NONBLOCK;
}
//---------------------------------------------------------------------------------------
void wrap_fcntl::set_nonblock( int fd, bool non )
{
    auto flags = get_status_control( fd );

    if (non) flags |= O_NONBLOCK;
    else     flags &= ~O_NONBLOCK;

    set_status_control( fd, flags );
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
