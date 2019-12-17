#include "wrap_unistd.h"

#include <unistd.h>
#include <fcntl.h> // Obtain O_* constant definitions

#include "impl_vposix/linux_call.h"
#include "impl_vposix/poll_context.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
void wrap_unistd::close( int fd )
{
    linux_call::check( ::close, fd );
}
//=======================================================================================
bool wrap_unistd::close_safe( int fd )
{
    if (fd < 0) return false;
    return 0 == linux_call::no_err( ::close, fd );
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
        auto sz = read( fd, buf, _read_buf_size );
        if ( sz < 0 )
        {
            ErrNo err;
            if ( err.resource_unavailable_try_again() ) break;
            err.do_throw( "wrap_unistd::read" );
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
    if ( res < 0 ) ErrNo().do_throw( "wrap_unistd::read" );
    return res;
}
//=======================================================================================
ssize_t wrap_unistd::read_no_err( int fd, void *buf, size_t buf_size )
{
    return linux_call::no_err( ::read, fd, buf, buf_size );
}
//=======================================================================================
void wrap_unistd::pipe2( int fds[] )
{
    linux_call::check( ::pipe2, fds, O_NONBLOCK );
}
//=======================================================================================
void wrap_unistd::dup2( int oldfd, int newfd )
{
    linux_call::check( ::dup2, oldfd, newfd );
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
    //  Надо убить котенка.
    execvp( cmd, const_cast<char**>(argv) );

    //  No return if OK.
    vfatal.nospace() << "Bad execvp call '" << cmd << "'";
    while( *argv )
        vfatal.nospace() << '\t' << *argv++;

    ErrNo().do_throw( "wrap_unistd::exec" );
}
//=======================================================================================


//=======================================================================================
safe_fd::safe_fd() noexcept
    : _fd( -1 )
{}
//---------------------------------------------------------------------------------------
safe_fd::safe_fd( int fd_ ) noexcept
    : _fd( fd_ )
{}
//---------------------------------------------------------------------------------------
safe_fd::~safe_fd()
{
    close();
}
//---------------------------------------------------------------------------------------
void safe_fd::close()
{
    if ( !has() ) return;

    if (_in_poll) poll_del();

    wrap_unistd::close( _fd );
    _fd = -1;
}
//---------------------------------------------------------------------------------------
impl_vposix::safe_fd::operator int() const noexcept
{
    return _fd;
}
//---------------------------------------------------------------------------------------
bool safe_fd::has() const noexcept
{
    return _fd >= 0;
}
//---------------------------------------------------------------------------------------
bool safe_fd::in_poll() const noexcept
{
    return _in_poll;
}
//---------------------------------------------------------------------------------------
safe_fd::safe_fd( safe_fd && other ) noexcept
{
    std::swap( _fd, other._fd );
    std::swap( _in_poll, other._in_poll );
}
//---------------------------------------------------------------------------------------
safe_fd &safe_fd::operator =( safe_fd && other ) noexcept
{
    std::swap( _fd, other._fd );
    std::swap( _in_poll, other._in_poll );
    return *this;
}
//---------------------------------------------------------------------------------------
safe_fd& safe_fd::operator = ( int other )
{
    close();
    _fd = other;
    return *this;
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_add_read( epoll_receiver *receiver )
{
    assert( !_in_poll );
    poll_context::current()->epoll.add( _fd, epoll::In, receiver );
    _in_poll = true;
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_add_write( epoll_receiver *receiver )
{
    assert( !_in_poll );
    poll_context::current()->epoll.add( _fd, epoll::Out, receiver );
    _in_poll = true;
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_add_rw( epoll_receiver *receiver )
{
    assert( !_in_poll );
    poll_context::current()->epoll.add( _fd, epoll::InOut, receiver );
    _in_poll = true;
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_mod_read( epoll_receiver *receiver )
{
    assert( _in_poll );
    poll_context::current()->epoll.mod( _fd, epoll::In, receiver );
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_mod_write( epoll_receiver *receiver )
{
    assert( _in_poll );
    poll_context::current()->epoll.mod( _fd, epoll::Out, receiver );
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_mod_rw( epoll_receiver *receiver )
{
    assert( _in_poll );
    poll_context::current()->epoll.mod( _fd, epoll::InOut, receiver );
}
//---------------------------------------------------------------------------------------
void safe_fd::poll_del()
{
    assert( _in_poll );
    poll_context::current()->epoll.del( _fd );
    _in_poll = false;
}
//=======================================================================================
