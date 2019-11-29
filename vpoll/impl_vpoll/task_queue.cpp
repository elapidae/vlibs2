#include "task_queue.h"

#include <cassert>

using namespace impl_vpoll;

//=======================================================================================

static thread_local task_queue cur;

//=======================================================================================
task_queue* impl_vpoll::task_queue::current()
{
    return & cur;
}
//=======================================================================================
void task_queue::push( vinvoke_iface::func_invokable && f )
{
    std::lock_guard<std::mutex> lock(_mutex );

    _queue.push( std::move(f) );
}
//=======================================================================================
vinvoke_iface::func_invokable task_queue::pop()
{
    std::lock_guard<std::mutex> lock(_mutex );

    assert( !_queue.empty() );

    auto res = _queue.front();
    _queue.pop();
    return res;
}
//=======================================================================================
