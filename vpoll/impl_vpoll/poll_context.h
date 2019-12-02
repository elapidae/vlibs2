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
        using task_type = vinvoke_iface::func_invokable;

        poll_context();
        ~poll_context() override;

        void push( task_type && task );

        void poll();
        void stop();

        //  for vthread, for checking poll thread.
        void recatch_thread_id();

    private:
        std::queue<task_type> tasks;
        std::mutex tasks_mutex;

        std::thread::id my_id;

        impl_vposix::eventfd semaphore;

        impl_vposix::epoll epoll;
        bool in_poll  { false };
        bool let_stop { false };

        void on_ready_read() override;
    };
    //===================================================================================
} // namespace impl_vpoll
//=======================================================================================

#endif // IMPL_VPOLL_POLL_CONTEXT_H
