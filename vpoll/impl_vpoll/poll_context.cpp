#include "poll_context.h"

#include <cassert>

#include "vlog.h"

using namespace impl_vpoll;

//=======================================================================================
poll_context *poll_context::current()
{
    static thread_local poll_context res;
    return &res;
}
//=======================================================================================
poll_context::poll_context()
{
    my_id = std::this_thread::get_id();
    epoll.add_read( semaphore.handle(), this );
}
//=======================================================================================
poll_context::~poll_context()
{
    epoll.del( semaphore.handle() );
}
//=======================================================================================
void poll_context::on_ready_read()
{
    while ( semaphore.read() )
    {
        task_type task { nullptr };
        {
            std::lock_guard<std::mutex> lock( tasks_mutex );
            assert( !tasks.empty() );
            task = std::move( tasks.front() );
            tasks.pop();
        }

        if (task)
        {
            task();
        }
        else
        {
            let_stop = true;
            break;              //  Сразу прерываем обработку очереди задач.
                                //  Не зря же нас попросили остановится.
        }
    } // while has task or null received.
}
//=======================================================================================
void poll_context::poll()
{
    if( in_poll )
        throw verror << "Recurse poll detected.";

    if( my_id != std::this_thread::get_id() )
        throw verror << "Poll in other thread";

    in_poll  = true;
    let_stop = false;
    try
    {
        while ( !let_stop )
            epoll.wait_once();
    }
    catch (...)
    {
        in_poll = false;
        throw;
    }

    in_poll = false;
}
//=======================================================================================
void poll_context::stop()
{
    //  Здесь могут быть вызовы из других потоков,
    //  нельзя трогать никаких не atomic переменных.
    push( nullptr );
}
//=======================================================================================
void poll_context::push( task_type && task )
{
    {
        std::lock_guard<std::mutex> lock( tasks_mutex );
        tasks.push( std::move(task) );
    }
    semaphore.write();
}
//=======================================================================================
size_t poll_context::tasks_count() const
{
    std::lock_guard<std::mutex> lock( tasks_mutex );
    return tasks.size();
}
//=======================================================================================
void poll_context::tasks_clear()
{
    std::lock_guard<std::mutex> lock( tasks_mutex );

    while ( semaphore.read() )
        tasks.pop();
}
//=======================================================================================
