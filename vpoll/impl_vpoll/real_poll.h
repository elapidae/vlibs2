#ifndef REAL_POLL_H
#define REAL_POLL_H

#include "impl_vposix/wrap_sys_epoll.h"


namespace impl_vpoll
{
    class real_poll
    {
        //-------------------------------------------------------------------------------
    public:

        static void poll();
        static void stop();

        //-------------------------------------------------------------------------------
        //  Используется только сервисными классами
        static void add_read  ( int fd, impl_vposix::epoll_receiver * receiver );
        static void add_write ( int fd, impl_vposix::epoll_receiver * receiver );
        static void add_rw    ( int fd, impl_vposix::epoll_receiver * receiver );

        static void mod_read  ( int fd, impl_vposix::epoll_receiver * receiver );
        static void mod_write ( int fd, impl_vposix::epoll_receiver * receiver );
        static void mod_rw    ( int fd, impl_vposix::epoll_receiver * receiver );

        static void del( int fd );

        //-------------------------------------------------------------------------------
    private:
        real_poll() = delete;
    };

} // namespace impl_vpoll

#endif // REAL_POLL_H
