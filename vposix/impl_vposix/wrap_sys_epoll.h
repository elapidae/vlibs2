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

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class epoll_receiver
    {
    public:
        virtual ~epoll_receiver();

        virtual void on_ready_read();
        virtual void on_ready_write();
        virtual void on_hang_up();
    };
    //===================================================================================
    class epoll final
    {
    public:
        epoll();
        ~epoll();

        void add_read   ( int fd, epoll_receiver * receiver );
        void add_write  ( int fd, epoll_receiver * receiver );
        void add_rw     ( int fd, epoll_receiver * receiver );

        void mod_read   ( int fd, epoll_receiver * receiver );
        void mod_write  ( int fd, epoll_receiver * receiver );
        void mod_rw     ( int fd, epoll_receiver * receiver );

        void del        ( int fd );

        void wait_once();

    private:
        int _efd;
        int _count = 0;
    };
    //===================================================================================

    //===================================================================================
    //  на все события, помимо In, Out выставляются флаги |EPOLLRDHUP|EPOLLPRI;
    //  efd -- epoll file descriptor.
    //  fd  -- controlled file descriptor.
    struct wrap_sys_epoll final
    {
        enum Direction { In, Out, InOut };

        static int create();

        static void add( int efd, int fd, Direction d, epoll_receiver * receiver );
        static void mod( int efd, int fd, Direction d, epoll_receiver * receiver );

        static void del( int efd, int fd );

        static void wait_once( int efd );
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_EPOLL_H
