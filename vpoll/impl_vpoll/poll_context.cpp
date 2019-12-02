#include "poll_context.h"

#include <cassert>

#include "vlog.h"

using namespace impl_vpoll;

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
void poll_context::recatch_thread_id()
{
    my_id = std::this_thread::get_id();
}
//=======================================================================================
void poll_context::on_ready_read()
{
    task_type task;
    while ( semaphore.read() )
    {
        {
            std::lock_guard<std::mutex> lock( tasks_mutex );
            assert( !tasks.empty() );
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
