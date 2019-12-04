#ifndef IMPL_VPOLL_POLL_CONTEXT_H
#define IMPL_VPOLL_POLL_CONTEXT_H

#include <queue>
#include <mutex>
#include <thread>

#include "vinvoke_iface.h"

#include "impl_vposix/wrap_sys_epoll.h"
#include "impl_vposix/wrap_sys_eventfd.h"


//=======================================================================================
namespace impl_vpoll
{
    //===================================================================================
    class poll_context final : public impl_vposix::epoll_receiver
    {
    public:
        using task_type = vinvoke_iface::task_type;

        static poll_context * current();

        poll_context();
        ~poll_context() override;

        void   push( task_type && task );
        size_t tasks_count() const;
        void   tasks_clear();

        void poll();    //  Сколько раз будет вызыван stop(), столько раз
        void stop();    //  будет прекращен poll().

        //  Для регистрации в системе поллинга, используется сервисными классами.
        //  Если надо чего-нибудь поллить, следует делать:
        //
        //  poll_context::current()->epoll.add_*( fd )
        //  poll_context::current()->epoll.mod_*( fd )
        //  poll_context::current()->epoll.del  ( fd )
        impl_vposix::epoll epoll;

    private:
        std::queue<task_type> tasks;
        mutable std::mutex tasks_mutex;

        std::thread::id my_id;

        impl_vposix::eventfd semaphore;

        bool in_poll  { false };
        bool let_stop { false };

        void on_ready_read() override;
    };
    //===================================================================================
} // namespace impl_vpoll
//=======================================================================================

#endif // IMPL_VPOLL_POLL_CONTEXT_H
