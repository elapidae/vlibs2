#include "poll_context.h"

#include <cassert>

#include "vlog.h"

using namespace impl_vposix;

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
    epoll.add( semaphore.handle(), epoll::In, this );
}
//=======================================================================================
poll_context::~poll_context()
{
    epoll.del( semaphore.handle(), this );
}
//=======================================================================================
void poll_context::on_events( impl_vposix::epoll_receiver::events e )
{
    bool r = e.take_read();     //  Для проверки, что нам прилетел
    assert(r); (void)r;         //  один и только один
    e.check_empty();            //  флаг на чтение.

    while ( semaphore.read() )
    {
        task_type task { nullptr };
        {
            std::lock_guard<std::mutex> lock( tasks_mutex );

            //  UPD 2023-08-01, depressed assert( !tasks.empty() )
            //  NB!!! The semaphore may not work correctly in docker!!!
            //        Sometimes false readings happen!!!
            if ( tasks.empty() )
            {
                break;
            }

            task = std::move( tasks.front() );
            tasks.pop();
        }

        if ( task )
        {
            task();
        }
        else
        {
            let_stop = true;    //  Сразу прерываем обработку очереди задач.
            break;              //  Не зря же нас попросили остановится.
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
