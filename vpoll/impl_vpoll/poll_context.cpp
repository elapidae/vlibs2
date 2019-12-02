#include "poll_context.h"

#include <queue>
#include <mutex>
#include <cassert>

#include "impl_vposix/wrap_sys_epoll.h"
#include "impl_vposix/wrap_sys_eventfd.h"

#include "vlog.h"

using namespace impl_vpoll;



//=======================================================================================
class poll_context::thread_local_pimpl final : public impl_vposix::epoll_receiver
{
public:
    thread_local_pimpl();
    ~thread_local_pimpl() override;

    std::queue<task_type> tasks;
    std::mutex tasks_mutex;

    impl_vposix::eventfd semaphore;

    impl_vposix::epoll poll;
    bool in_poll  { false };
    bool let_stop { false };

    void on_ready_read() override;
};
//-----------------------------------------------------------
static poll_context::thread_local_pimpl & context()
{
    static thread_local poll_context::thread_local_pimpl res;
    return res;
}
//-----------------------------------------------------------

//=======================================================================================
// static
void poll_context::poll()
{
    auto & p = context();

    if( p.in_poll )
        throw verror << "Recurse poll detected.";

    p.in_poll  = true;

    p.let_stop = false;
    try
    {
        while ( !p.let_stop )
            p.poll.wait_once();

    }
    catch (...)
    {
        p.in_poll = false;
        throw;
    }

    p.in_poll = false;
}
//=======================================================================================
poll_context::thread_local_pimpl::thread_local_pimpl()
{
    poll.add_read( semaphore.handle(), this );
}
//=======================================================================================
poll_context::thread_local_pimpl::~thread_local_pimpl()
{
    poll.del( semaphore.handle() );
}
//=======================================================================================
void poll_context::thread_local_pimpl::on_ready_read()
{
    task_type task;
    while ( semaphore.read() )
    {
        {
            std::lock_guard<std::mutex> lock( tasks_mutex );
            task = std::move( tasks.front() );
            tasks.pop();
        }

        if ( task )
            task();
        else
            let_stop = true;

    } // while has task.
}
//=======================================================================================
void poll_context::stop()
{
    push( nullptr );
}
//=======================================================================================
poll_context::poll_context()
    : p( &context() )
{}
//=======================================================================================
void poll_context::push( task_type && task )
{
    {
        std::lock_guard<std::mutex> lock( p->tasks_mutex );
        p->tasks.push( std::move(task) );
    }
    p->semaphore.write();
}
//=======================================================================================
