#include "vapplication.h"

#include <cassert>
#include <mutex>

#include "vlog.h"
#include "impl_vpoll/poll_context.h"

using namespace impl_vpoll;

//=======================================================================================

//----------------------------------------
//  Can be only as static variable.
static poll_context first_ctx;
//----------------------------------------

//=======================================================================================
void vapplication::_invoke( func_invokable && f )
{
    first_ctx.push( std::move(f) );
}
//=======================================================================================
void vapplication::poll()
{
    first_ctx.poll();
}
//=======================================================================================
void vapplication::stop()
{
    first_ctx.stop();
}
//=======================================================================================
