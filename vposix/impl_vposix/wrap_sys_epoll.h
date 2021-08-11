#ifndef IMPL_VPOSIX_WRAP_EPOLL_H
#define IMPL_VPOSIX_WRAP_EPOLL_H

//=======================================================================================
/*
 *  2019-11-26  by elapidae
 *
 *  http://man7.org/linux/man-pages/man7/epoll.7.html
 *
*/
//=======================================================================================

#include <stdint.h>
#include <string>
#include <map>
#include <functional>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class epoll_receiver
    {
    public:
        class events;

        virtual ~epoll_receiver();

        virtual void on_events( events ) = 0;
    };
    //===================================================================================
    class epoll_receiver::events
    {
    public:
        bool take_read();           //  EPOLLIN
        bool take_write();          //  EPOLLOUT
        bool take_hang_up();        //  EPOLLHUP
        bool take_read_hang_up();   //  EPOLLRDHUP
        bool take_error();          //  EPOLLERR

        void check_empty();         //  throws verror if not all flags extarcted.

    private:
        std::string _leaved() const;
        bool _take( uint32_t flag );

        friend struct wrap_sys_epoll; events( uint32_t e );
        uint32_t _ev;
    };
    //===================================================================================
    class epoll final
    {
    public:
        enum Direction { In, Out, InOut };

        epoll();
        ~epoll();

        void add( int fd, epoll::Direction d, epoll_receiver *receiver );
        void mod( int fd, epoll::Direction d, epoll_receiver *receiver );

        void del        ( int fd );

        void wait_once  ();

    private:
        int _efd;
        int _count = 0;

        std::map<int, void*> _polled;
        bool is_polled(void* rcv) const;
        bool is_polled(int fd) const;
    };
    //===================================================================================

    //===================================================================================
    //  на все события, помимо In, Out выставляются флаги |EPOLLRDHUP|EPOLLPRI;
    //  efd -- epoll file descriptor.
    //  fd  -- controlled file descriptor.
    struct wrap_sys_epoll final
    {
        static int create();

        static void add( int efd, int fd, epoll::Direction d, epoll_receiver *receiver );
        static void mod( int efd, int fd, epoll::Direction d, epoll_receiver *receiver );

        static void del( int efd, int fd );

        static void wait_once( int efd, std::function<bool(void*)> checker );     //  зачем этот враппер и к тому же в хидере?
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_EPOLL_H
