#include "vthread.h"

#include <future>
#include <queue>
#include <cassert>
#include <mutex>
#include <atomic>

#include "vlog.h"
#include "impl_vpoll/poll_context.h"

using namespace impl_vpoll;


//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class vthread::_pimpl
{
public:
    _pimpl( task_type&& alternate_func_ );

    std::future<void>   future;
    task_type           alternate_func { nullptr };

    std::atomic_bool    started { false };
    std::atomic_bool    joined  { false };

    poll_context       *my_ctx  { nullptr };

    std::mutex          run_mutex;
};
#pragma GCC diagnostic pop
//---------------------------------------------------------------------------------------
vthread::_pimpl::_pimpl( task_type&& alternate_func_ )
    : alternate_func( alternate_func_ )
{
    future = std::async( std::launch::async, _run, this );
}
//=======================================================================================
void vthread::_run( _pimpl *p )
{
    p->my_ctx  = poll_context::current();
    p->started = true;

    try
    {
        if ( p->alternate_func )
            p->alternate_func();
        else
            p->my_ctx->poll();
    }
    catch (...)
    {
        p->run_mutex.lock();
        p->my_ctx = nullptr;
        throw;
    }
    p->run_mutex.lock();
    p->my_ctx = nullptr;
}
//=======================================================================================
void vthread::poll()
{
    poll_context::current()->poll();
}
//=======================================================================================
vthread::vthread( task_type alternate_func )
    : _p( new _pimpl(std::move(alternate_func)) )
{
    while ( !_p->started )
    {
        std::this_thread::yield();
    }
}
//=======================================================================================
vthread::~vthread() noexcept(false)
{
    if ( _p->joined ) return;

    join();
}
//=======================================================================================
void vthread::join()
{
    assert( !_p->joined );

    //  Если смогли захватить мьютекс, значит пока не вышли из поллинга и не дадим методу
    //  _run завершиться, тем самым сможем безопасно отправить my_ctx->stop().
    if ( _p->run_mutex.try_lock() )
    {
        _p->my_ctx->stop();
        _p->run_mutex.unlock();
    }

    _p->joined = true;
    _p->future.get();
}
//=======================================================================================
size_t vthread::tasks_count() const
{
    assert( !_p->joined );
    return _p->my_ctx->tasks_count();
}
//=======================================================================================
void vthread::tasks_clear()
{
    assert( !_p->joined );
    return _p->my_ctx->tasks_clear();
}
//=======================================================================================
void vthread::_invoke( vthread::task_type && func )
{
    assert( !_p->joined );

    _p->my_ctx->push( std::move(func) );
}
//=======================================================================================
