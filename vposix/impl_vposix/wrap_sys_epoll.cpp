#include "impl_vposix/wrap_sys_epoll.h"

#include <sys/epoll.h>
#include "impl_vposix/linux_call.h"
#include "impl_vposix/wrap_unistd.h"
#include "vlog.h"

using namespace impl_vposix;

//=======================================================================================
//  Шпаргалки.
//=======================================================================================
//EPOLLRDHUP (since Linux 2.6.17)
//              Stream socket peer closed connection, or shut down writing
//              half of connection.  (This flag is especially useful for writ‐
//              ing simple code to detect peer shutdown when using Edge Trig‐
//              gered monitoring.)
//
//EPOLLHUP
//              Hang up happened on the associated file descriptor.
//              epoll_wait(2) will always wait for this event; it is not nec‐
//              essary to set it in events.
//
//              Note that when reading from a channel such as a pipe or a
//              stream socket, this event merely indicates that the peer
//              closed its end of the channel.  Subsequent reads from the
//              channel will return 0 (end of file) only after all outstanding
//              data in the channel has been consumed.
//=======================================================================================


//=======================================================================================
//      epoll_receiver, throws not implemented message
//=======================================================================================
impl_vposix::epoll_receiver::~epoll_receiver()
{}
//---------------------------------------------------------------------------------------
void epoll_receiver::on_many_flags( epoll_receiver::events )
{
    throw verror << "not implemented";
}
//---------------------------------------------------------------------------------------
//  EPOLLIN
void impl_vposix::epoll_receiver::on_ready_read()
{
    throw verror << "not implemented";
}
//---------------------------------------------------------------------------------------
//  EPOLLOUT
void impl_vposix::epoll_receiver::on_ready_write()
{
    throw verror << "not implemented";
}
//---------------------------------------------------------------------------------------
//  EPOLLRDHUP
void impl_vposix::epoll_receiver::on_peer_shut_down_writing()
{
    throw verror << "not implemented";
}
//=======================================================================================

//=======================================================================================
//      epoll as normal class
//=======================================================================================
epoll::epoll()
    : _efd( wrap_sys_epoll::create() )
{}
//=======================================================================================
epoll::~epoll()
{
    //  Никаких исключений в деструкторе.
    if ( _count != 0 )
        vfatal << "Bad counter for add/del handles in epoll:" << _count;

    wrap_unistd::close( _efd );
}
//=======================================================================================
void epoll::add_read( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, wrap_sys_epoll::In, receiver );
    ++_count;
}
//=======================================================================================
void epoll::add_write( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, wrap_sys_epoll::Out, receiver );
    ++_count;
}
//=======================================================================================
void epoll::add_rw( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, wrap_sys_epoll::InOut, receiver );
    ++_count;
}
//=======================================================================================
void epoll::mod_read( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, wrap_sys_epoll::In, receiver );
}
//=======================================================================================
void epoll::mod_write( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, wrap_sys_epoll::Out, receiver );
}
//=======================================================================================
void epoll::mod_rw( int fd, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, wrap_sys_epoll::InOut, receiver );
}
//=======================================================================================
void epoll::del( int fd )
{
    wrap_sys_epoll::del( _efd, fd );
    --_count;
}
//=======================================================================================
void epoll::wait_once()
{
    wrap_sys_epoll::wait_once( _efd );
}
//=======================================================================================

//=======================================================================================
//      wrap_sys_epoll, only wrappers for kernel calls
//=======================================================================================
//  http://man7.org/linux/man-pages/man2/epoll_create.2.html
int wrap_sys_epoll::create()
{
    return linux_call::check( ::epoll_create1, EPOLL_CLOEXEC );
}
//=======================================================================================
static void econtrol( int efd,
                      int fd,
                      int operation,
                      uint32_t events,
                      epoll_receiver* receiver )
{
    epoll_event event;
    event.data.ptr = receiver;
    event.events   = events|EPOLLRDHUP|EPOLLPRI;
    linux_call::check( ::epoll_ctl, efd, operation, fd, &event );
}
//---------------------------------------------------------------------------------------
static uint32_t direction( wrap_sys_epoll::Direction d )
{
    switch (d)
    {
    case wrap_sys_epoll::In:    return EPOLLIN;
    case wrap_sys_epoll::Out:   return EPOLLOUT;
    case wrap_sys_epoll::InOut: return EPOLLIN|EPOLLOUT;
    }
    throw verror( "Direction not delected" );
}
//=======================================================================================
void wrap_sys_epoll::add( int efd, int fd, wrap_sys_epoll::Direction d,
                          epoll_receiver *receiver )
{
    assert( receiver );
    econtrol( efd, fd, EPOLL_CTL_ADD, direction(d), receiver );
}
//=======================================================================================
void wrap_sys_epoll::mod( int efd, int fd, wrap_sys_epoll::Direction d,
                          epoll_receiver *receiver )
{
    assert( receiver );
    econtrol( efd, fd, EPOLL_CTL_MOD, direction(d), receiver );
}
//=======================================================================================
void wrap_sys_epoll::del( int efd, int fd )
{
    econtrol( efd, fd, EPOLL_CTL_DEL, 0, nullptr );
}
//=======================================================================================
void wrap_sys_epoll::wait_once( int efd )
{
    enum { Waits_Count = 16 };

    struct epoll_event events[Waits_Count];
    int count = linux_call::check( ::epoll_wait, efd, events, Waits_Count, -1 );

    for ( int i = 0; i < count; ++i )
    {
        epoll_receiver *receiver = static_cast<epoll_receiver*>( events[i].data.ptr );
        uint32_t event = events[i].events;

        switch( event )
        {
        case EPOLLIN:       receiver->on_ready_read();              break;
        case EPOLLOUT:      receiver->on_ready_write();             break;
        case EPOLLRDHUP:    receiver->on_peer_shut_down_writing();  break;
        default:
            receiver->on_many_flags( epoll_receiver::events(event) );
        }

    }
}
//=======================================================================================


//=======================================================================================
epoll_receiver::events::events( uint32_t e )
    : _events(e)
{}
//---------------------------------------------------------------------------------------
//  NB! non const reference, здесь так удобнее.
static bool get_clear_flag( uint32_t& ev, uint32_t flag )
{
    bool res = ev & flag;
    ev &= ~flag;
    return res;
}
//---------------------------------------------------------------------------------------
bool epoll_receiver::events::take_read()
{
    return get_clear_flag( _events, EPOLLIN );
}
//---------------------------------------------------------------------------------------
bool epoll_receiver::events::take_write()
{
    return get_clear_flag( _events, EPOLLOUT );
}
//---------------------------------------------------------------------------------------
bool epoll_receiver::events::take_read_hang_up()
{
    return get_clear_flag( _events, EPOLLRDHUP );
}
//---------------------------------------------------------------------------------------
bool epoll_receiver::events::take_hang_up()
{
    return get_clear_flag( _events, EPOLLHUP );
}
//---------------------------------------------------------------------------------------
bool epoll_receiver::events::take_error()
{
    return get_clear_flag( _events, EPOLLERR );
}
//---------------------------------------------------------------------------------------
bool epoll_receiver::events::empty() const
{
    return _events == 0;
}
//---------------------------------------------------------------------------------------
void epoll_receiver::events::throw_if_need( const std::string& src )
{
    if ( !empty() ) throw verror( src, ": not all events from EPOLL." );
}
//=======================================================================================

