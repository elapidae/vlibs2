#include "vthread.h"

#include <future>
#include <queue>
#include <cassert>
#include <mutex>

#include "vlog.h"
#include "impl_vpoll/poll_context.h"

using namespace impl_vpoll;


//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class vthread::_pimpl
{
public:
    _pimpl( func_invokable alternate_func_, std::atomic_bool *started );

    std::future<void> future;
    func_invokable    alternate_func { nullptr };

    std::atomic_bool  joined  { false };

    poll_context      my_ctx;
};
#pragma GCC diagnostic pop

//-------------------------------------------------------
static thread_local poll_context * current_ctx {nullptr};
//-------------------------------------------------------

//---------------------------------------------------------------------------------------
vthread::_pimpl::_pimpl( func_invokable alternate_func_, std::atomic_bool *started )
    : alternate_func( alternate_func_ )
{
    future = std::async( std::launch::async, _run, this, started );
}
//=======================================================================================
void vthread::_run( _pimpl *p , std::atomic_bool *started )
{
    p->my_ctx.recatch_thread_id();
    current_ctx = &p->my_ctx;
    *started = true;

    try
    {
        if ( p->alternate_func )
            p->alternate_func();
        else
            p->my_ctx.poll();
    }
    catch (...)
    {
        current_ctx = nullptr;
        throw;
    }
    current_ctx = nullptr;
}
//=======================================================================================
void vthread::poll()
{
    assert( current_ctx );
    current_ctx->poll();
}
//=======================================================================================
vthread::vthread( func_invokable alternate_func )
{
    std::atomic_bool started { false };

    _p.reset( new _pimpl(alternate_func, &started) );

    while ( !started )
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
    _p->my_ctx.stop();

    _p->joined = true;  //  сначала выставляем флаг, т.к. _invoke его проверяет;
    _p->future.get();   //  и только теперь завершаем поток, если будет исключение,
                        //  то флаг уже выставлен;
}
//=======================================================================================
void vthread::_invoke( vthread::func_invokable && func )
{
    assert( !_p->joined );

    _p->my_ctx.push( std::move(func) );
}
//=======================================================================================
