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
#include <memory>


//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class epoll_receiver
    {
    public:
        class events;

        virtual ~epoll_receiver() = default;

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
        void del( int fd, epoll_receiver *receiver );

        void wait_once();

        struct triggered_events;

    private:
        int _efd;
        int _count = 0;

        std::unique_ptr<triggered_events> _triggered_events;
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_EPOLL_H
