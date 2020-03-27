#include "wrap_unistd.h"

#include <unistd.h>
#include <fcntl.h> // Obtain O_* constant definitions

#include "impl_vposix/linux_call.h"
//#include "impl_vposix/poll_context.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
void wrap_unistd::close( int fd )
{
    linux_call::check( ::close, fd );
}
//=======================================================================================
void wrap_unistd::usleep( uint usec )
{
    ::usleep( usec );
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
std::string wrap_unistd::read( int fd )
{
    char buf [ _read_buf_size ];

    std::string res;
    while (1)
    {
        auto sz = linux_call::no_err( ::read, fd, buf, _read_buf_size );
        if ( sz < 0 )
        {
            ErrNo err;
            if ( err.resource_unavailable_try_again() ) break;
            err.do_throw( "wrap_unistd::read(1)" );
        }
        res.append( buf, size_t(sz) );
        if ( size_t(sz) < _read_buf_size ) break;
    }
    return res;
}
//=======================================================================================
ssize_t wrap_unistd::read( int fd, void *buf, size_t buf_size )
{
    auto res = linux_call::no_err( ::read, fd, buf, buf_size );
    if ( res < 0 ) ErrNo().do_throw( "wrap_unistd::read(2)" );
    return res;
}
//=======================================================================================
ssize_t wrap_unistd::read_no_err( int fd, void *buf, size_t buf_size )
{
    return linux_call::no_err( ::read, fd, buf, buf_size );
}
//=======================================================================================
bool wrap_unistd::unlink_no_err( const std::string& filename )
{
    return 0 == linux_call::no_err( ::unlink, filename.c_str() );
}
//=======================================================================================
void wrap_unistd::pipe_non_block( int fds[] )
{
    linux_call::check( ::pipe2, fds, O_NONBLOCK );
}
//=======================================================================================
void wrap_unistd::pipe( int fds[] )
{
    linux_call::check( ::pipe, fds );
}
//=======================================================================================
void wrap_unistd::dup2( int oldfd, int newfd )
{
    auto res = linux_call::check( ::dup2, oldfd, newfd );
    assert( res == newfd );
}
//=======================================================================================
int wrap_unistd::dup( int clone_fd )
{
    return linux_call::check( ::dup, clone_fd );
}
//=======================================================================================
int wrap_unistd::fork()
{
    return linux_call::check( ::fork );
}
//=======================================================================================
//  http://man7.org/linux/man-pages/man2/execve.2.html
void wrap_unistd::exec( const char *cmd, const char * const * argv )
{
    //  Надо убить котенка, скастовать.
    execvp( cmd, const_cast<char**>(argv) );

    //  No return if OK.
    vfatal.nospace() << "Bad execvp call '" << cmd << "'";
    while( *argv )
        vfatal.nospace() << '\t' << *argv++;

    ErrNo().do_throw( "wrap_unistd::exec" );
}
//=======================================================================================
pid_t wrap_unistd::get_pid() noexcept
{
    return ::getpid();
}
//=======================================================================================
pid_t wrap_unistd::get_parent_pid() noexcept
{
    return ::getppid();
}
//=======================================================================================
pid_t wrap_unistd::get_pid_group() noexcept
{
    return ::getpgrp();
}
//=======================================================================================
