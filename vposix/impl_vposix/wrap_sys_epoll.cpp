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
namespace impl_vposix
{
    //===================================================================================
    //  UPD 2022-12-xx by elapidae
    //
    //  Здесь решается следующая проблема:
    //   - пусть у нас есть управляющий и управляемый polled объекты;
    //   - управляющий объект может удалить управляемый при вызове on_events();
    //   - управляемый объект также попал в пул событий после управляющего;
    //
    //  При появлении всех этих условий, имеем вызов управляемого объекта уже после его
    //  удаления. Чтобы решить эту проблему, при удалении и изменении условий поллинга,
    //  будем очищать совпадающие объекты из текущего результата поллинга.
    struct epoll::triggered_events final
    {
        enum { waits_count = 64 };
        struct epoll_event events[ waits_count ];

        int cur = 0;    //  Idexes will be iterate in wait_once() method.
        int end = 0;    //

        void drop( void * ptr )
        {
            for ( int i = cur; i < end; ++i )
            {
                if ( events[i].data.ptr != ptr )
                    continue;

                events[i].data.ptr = nullptr;
                break;
            }
        }
    };
    //-----------------------------------------------------------------------------------
    //  UPD 2022-12-16 by elapidae
    //  Перенесено из хедера сюда, чтобы можно было поллить, сбрасывая уже невалидные
    //  указатели.
    //
    //  на все события, помимо In, Out выставляются флаги |EPOLLRDHUP|EPOLLPRI;
    //  efd -- epoll file descriptor.
    //  fd  -- controlled file descriptor.
    struct wrap_sys_epoll final
    {
        static int create();

        static void add( int efd, int fd, epoll::Direction d, epoll_receiver *receiver );
        static void mod( int efd, int fd, epoll::Direction d, epoll_receiver *receiver );

        static void del( int efd, int fd );

        static void wait_once( int efd, epoll::triggered_events * events );
    };
} // namespace impl_vposix
//=======================================================================================


//=======================================================================================
//      epoll as normal class
//=======================================================================================
epoll::epoll()
    : _efd( wrap_sys_epoll::create() )
    , _triggered_events( new triggered_events )
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
void epoll::add( int fd, epoll::Direction d, epoll_receiver *receiver )
{
    wrap_sys_epoll::add( _efd, fd, d, receiver );
    ++_count;
}
//=======================================================================================
void epoll::mod( int fd, epoll::Direction d, epoll_receiver *receiver )
{
    _triggered_events->drop( receiver );
    wrap_sys_epoll::mod( _efd, fd, d, receiver );
}
//=======================================================================================
void epoll::del( int fd, epoll_receiver *receiver )
{
    _triggered_events->drop( receiver );
    wrap_sys_epoll::del( _efd, fd );
    --_count;
}
//=======================================================================================
void epoll::wait_once()
{
    *_triggered_events = {};
    wrap_sys_epoll::wait_once( _efd, _triggered_events.get() );
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
void wrap_sys_epoll::wait_once( int efd, epoll::triggered_events * evs )
{
    evs->end = linux_call::check( ::epoll_wait, efd, evs->events, evs->waits_count, -1 );


    for ( evs->cur = 0; evs->cur < evs->end; ++evs->cur )
    {
        auto event = evs->events[ evs->cur ];
        if ( !event.data.ptr ) continue;

        epoll_receiver *receiver = static_cast<epoll_receiver*>( event.data.ptr );
        receiver->on_events( {event.events} );
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
        throw verror.hex() << "Internal error, epoll flags not used at all: "
                           << _leaved() << " [" << _ev << "]";
}
//---------------------------------------------------------------------------------------
std::string epoll_receiver::events::_leaved() const
{
    vcat res;
    if ( _ev & EPOLLIN    ) res("|IN");
    if ( _ev & EPOLLOUT   ) res("|OUT");
    if ( _ev & EPOLLERR   ) res("|ERR");
    if ( _ev & EPOLLHUP   ) res("|HANG");
    if ( _ev & EPOLLRDHUP ) res("|READ_HANG");
    return res << "|";
}
//=======================================================================================
//      epoll_receiver::events
//=======================================================================================
