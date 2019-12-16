#include "impl_vposix/wrap_sys_epoll.h"

#include <sys/epoll.h>
#include "impl_vposix/linux_call.h"
#include "impl_vposix/wrap_unistd.h"
#include "vlog.h"
#include "vcat.h"

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

    wrap_unistd::close_safe( _efd );
}
//=======================================================================================
void epoll::add( int fd, epoll::Direction d, epoll_receiver *receiver )
{
//    vdeb["poll"] << "add to epoll\t" << receiver << "\n";
    wrap_sys_epoll::add( _efd, fd, d, receiver );
    ++_count;
}
//=======================================================================================
void epoll::mod( int fd, epoll::Direction d, epoll_receiver *receiver )
{
    wrap_sys_epoll::mod( _efd, fd, d, receiver );
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
static uint32_t direction( epoll::Direction d )
{
    switch (d)
    {
    case epoll::In:    return EPOLLIN;
    case epoll::Out:   return EPOLLOUT;
    case epoll::InOut: return EPOLLIN|EPOLLOUT;
    }
    throw verror( "Direction not delected" );
}
//=======================================================================================
void wrap_sys_epoll::add( int efd, int fd, epoll::Direction d, epoll_receiver *receiver )
{
    assert( receiver );
    econtrol( efd, fd, EPOLL_CTL_ADD, direction(d), receiver );
}
//=======================================================================================
void wrap_sys_epoll::mod( int efd, int fd, epoll::Direction d, epoll_receiver *receiver )
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
//  Надо для отладки, пока что не уверен в поллинге.
//static std::string deb_flags( epoll_receiver::events f )
//{
//    vcat res;
//    if ( f.take_read() )        res("|IN");
//    if ( f.take_write() )       res("|OUT");
//    if ( f.take_error() )       res("|ERR");
//    if ( f.take_hang_up() )     res("|HANG");
//    if ( f.take_read_hang_up()) res("|READ_HANG");
//    return res;
//}
//---------------------------------------------------------------------------------------
void wrap_sys_epoll::wait_once( int efd )
{
    enum { waits_count = 16 };

    struct epoll_event events[waits_count];
    int count = linux_call::check( ::epoll_wait, efd, events, waits_count, -1 );

    for ( int i = 0; i < count; ++i )
    {
        epoll_receiver *receiver = static_cast<epoll_receiver*>( events[i].data.ptr );
        //uint32_t evs = events[i].events;
        //vdeb.hex() << "epolled" << receiver << evs << deb_flags(evs);
        receiver->on_events( {events[i].events} );
    }
}
//=======================================================================================


//=======================================================================================
//      epoll_receiver::events
//=======================================================================================
epoll_receiver::events::events( uint32_t e )
    : _ev( e )
{}
//=======================================================================================
bool epoll_receiver::events::_take( uint32_t flag )
{
    bool res = _ev & flag;
    _ev &= ~flag;
    return res;
}
//=======================================================================================
bool epoll_receiver::events::take_read()
{
    return _take( EPOLLIN );
}
//=======================================================================================
bool epoll_receiver::events::take_write()
{
    return _take( EPOLLOUT );
}
//=======================================================================================
bool epoll_receiver::events::take_hang_up()
{
    return _take( EPOLLHUP );
}
//=======================================================================================
bool epoll_receiver::events::take_read_hang_up()
{
    return _take( EPOLLRDHUP );
}
//=======================================================================================
bool epoll_receiver::events::take_error()
{
    return _take( EPOLLERR );
}
//=======================================================================================
void epoll_receiver::events::check_empty()
{
    if ( _ev != 0 )
        throw verror.hex() << "Internal error, epoll flags not used at all: " << _ev;
}
//=======================================================================================
//      epoll_receiver::events
//=======================================================================================
