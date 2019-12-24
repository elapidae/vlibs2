#include "safe_fd.h"

#include <cassert>
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/poll_context.h"

using namespace impl_vposix;

//=======================================================================================
safe_fd::safe_fd() noexcept
    : _fd( -1 )
{}
//=======================================================================================
safe_fd::safe_fd( int fd_ ) noexcept
    : _fd( fd_ )
{}
//=======================================================================================
safe_fd::~safe_fd()
{
    close();
}
//=======================================================================================
void safe_fd::close()
{
    if ( !has() ) return;

    if (_in_poll) poll_del();

    wrap_unistd::close( _fd );
    _fd = -1;
}
//=======================================================================================
impl_vposix::safe_fd::operator int() const noexcept
{
    return _fd;
}
//=======================================================================================
bool safe_fd::has() const noexcept
{
    return _fd >= 0;
}
//=======================================================================================
bool safe_fd::in_poll() const noexcept
{
    return _in_poll;
}
//=======================================================================================
safe_fd::safe_fd( safe_fd && other ) noexcept
{
    std::swap( _fd, other._fd );
    std::swap( _in_poll, other._in_poll );
}
//=======================================================================================
safe_fd &safe_fd::operator =( safe_fd && other ) noexcept
{
    std::swap( _fd, other._fd );
    std::swap( _in_poll, other._in_poll );
    return *this;
}
//=======================================================================================
safe_fd& safe_fd::operator = ( int other )
{
    close();
    _fd = other;
    return *this;
}
//=======================================================================================
void safe_fd::poll_add_read( epoll_receiver *receiver )
{
    assert( !_in_poll );
    poll_context::current()->epoll.add( _fd, epoll::In, receiver );
    _in_poll = true;
}
//=======================================================================================
void safe_fd::poll_add_write( epoll_receiver *receiver )
{
    assert( !_in_poll );
    poll_context::current()->epoll.add( _fd, epoll::Out, receiver );
    _in_poll = true;
}
//=======================================================================================
void safe_fd::poll_add_rw( epoll_receiver *receiver )
{
    assert( !_in_poll );
    poll_context::current()->epoll.add( _fd, epoll::InOut, receiver );
    _in_poll = true;
}
//=======================================================================================
void safe_fd::poll_mod_read( epoll_receiver *receiver )
{
    assert( _in_poll );
    poll_context::current()->epoll.mod( _fd, epoll::In, receiver );
}
//=======================================================================================
void safe_fd::poll_mod_write( epoll_receiver *receiver )
{
    assert( _in_poll );
    poll_context::current()->epoll.mod( _fd, epoll::Out, receiver );
}
//=======================================================================================
void safe_fd::poll_mod_rw( epoll_receiver *receiver )
{
    assert( _in_poll );
    poll_context::current()->epoll.mod( _fd, epoll::InOut, receiver );
}
//=======================================================================================
void safe_fd::poll_del()
{
    assert( _in_poll );
    poll_context::current()->epoll.del( _fd );
    _in_poll = false;
}
//=======================================================================================
