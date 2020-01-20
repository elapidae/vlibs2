#include "vthread_context.h"

#include "impl_vposix/poll_context.h"

//=======================================================================================
vthread_context::vthread_context()
{
    _my_ctx = impl_vposix::poll_context::current();
}
//=======================================================================================
size_t vthread_context::tasks_count() const
{
    auto * p = static_cast<impl_vposix::poll_context*>( _my_ctx );
    return p->tasks_count();
}
//=======================================================================================
void vthread_context::tasks_clear()
{
    auto * p = static_cast<impl_vposix::poll_context*>( _my_ctx );
    return p->tasks_clear();
}
//=======================================================================================
void vthread_context::_invoke( vinvoke_iface::task_type && task )
{
    auto * p = static_cast<impl_vposix::poll_context*>( _my_ctx );
    return p->push( std::move(task) );
}
//=======================================================================================
