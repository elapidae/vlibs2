#include "vthread.h"

#include <future>
#include <queue>
#include <cassert>
#include <mutex>
#include "impl_vpoll/real_poll.h"
#include "impl_vposix/wrap_sys_eventfd.h"
#include "impl_vposix/wrap_unistd.h"

using namespace impl_vposix;
using namespace impl_vpoll;

//=======================================================================================
using task_queue_type = std::queue<vthread::func_invokable>;
static thread_local task_queue_type tasks_queue;
//---------------------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
#pragma GCC diagnostic ignored "-Wpadded"
class vthread::_pimpl : public epoll_receiver
{
public:
    _pimpl( func_invokable alternate_func_ );

    void on_ready_read() override; // on_ready_read()

    std::future<void> future;

    func_invokable alternate_func;

    std::mutex queue_mutex;
    task_queue_type * volatile my_queue { nullptr };

    eventfd semaphore;

    bool joined = false;
};
#pragma GCC diagnostic pop
//---------------------------------------------------------------------------------------
#include "vlog.h"
vthread::_pimpl::_pimpl( func_invokable alternate_func_ )
    : alternate_func( alternate_func_ )
{
    vdeb << "about to async";
    future = std::async( std::launch::async, _run, this );
}
//---------------------------------------------------------------------------------------
void vthread::_pimpl::on_ready_read()
{
    assert( my_queue );

    while ( semaphore.read() )
    {
        func_invokable func;
        {
            std::lock_guard<std::mutex> lock( queue_mutex );
            func = my_queue->front();
            my_queue->pop();
        }

        if (!func)
            real_poll::stop();  // nullptr as stop marker.
        else
            func();

    } // while has data.
}
//---------------------------------------------------------------------------------------
//  Введен для того, чтобы автоматически контролировать постановку и снятие на поллинг.
struct in_thread_ctl final
{
    in_thread_ctl( vthread::_pimpl *p )
        : sem_fd( p->semaphore.handle() )
    {
        {   // set correct thread local ptr.
            std::lock_guard<std::mutex> lock( p->queue_mutex );
            p->my_queue = &tasks_queue;
        }

        real_poll::add_read( sem_fd, p );
    }

    ~in_thread_ctl()
    {
        real_poll::del( sem_fd );
    }
private:
    int sem_fd;
};
//=======================================================================================
void vthread::_run( vthread::_pimpl *p )
{
    vdeb << "in run";
    in_thread_ctl ctl( p );

    if ( p->alternate_func )
        p->alternate_func();
    else
        real_poll::poll(); // in my thread :)
}

//=======================================================================================
vthread::vthread( func_invokable alternate_func )
    : _p( new _pimpl(alternate_func) )
{}
//=======================================================================================
vthread::~vthread() noexcept(false)
{
    join();
}
//=======================================================================================
void vthread::join()
{
    if ( _p->joined ) return;

    _invoke( nullptr ); //  сначала посылаем терминатор;
    _p->joined = true;  //  потом выставляем флаг, т.к. _invoke его проверяет;
    _p->future.get();   //  и только теперь завершаем поток, если будет исключение,
                        //  то флаг уже выставлен;
}
//=======================================================================================
void vthread::invoke( vthread::func_invokable func )
{
    assert( func );
    _invoke( std::move(func) );
}
//=======================================================================================
void vthread::_invoke( vthread::func_invokable && func )
{
    assert( !_p->joined );

    //  Waiting before thread start.
    while ( 1 )
    {
        {
            std::lock_guard<std::mutex> lock( _p->queue_mutex );
            if ( _p->my_queue ) break;
        }
        wrap_unistd::usleep(1);
    }

    {
        std::lock_guard<std::mutex> lock( _p->queue_mutex );
        _p->my_queue->push( std::move(func) );
    }
    _p->semaphore.write();
}
//=======================================================================================
