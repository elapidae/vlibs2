#include "vapplication.h"

#include <cassert>
#include <mutex>

#include "vlog.h"
#include "impl_vpoll/poll_context.h"

using namespace impl_vpoll;

//=======================================================================================

//----------------------------------------
//  Can be only as static variable.
static poll_context *first_ctx = poll_context::current();
//----------------------------------------

//=======================================================================================
void vapplication::_invoke( task_type && f )
{
    first_ctx->push( std::move(f) );
}
//=======================================================================================
size_t vapplication::tasks_count() const
{
    return first_ctx->tasks_count();
}
//=======================================================================================
void vapplication::tasks_clear()
{
    first_ctx->tasks_clear();
}
//=======================================================================================
void vapplication::poll()
{
    first_ctx->poll();
}
//=======================================================================================
void vapplication::stop()
{
    first_ctx->stop();
}
//=======================================================================================
