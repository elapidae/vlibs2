#include "vapplication.h"

#include <cassert>
#include <mutex>

#include "impl_vpoll/poll_context.h"

using namespace impl_vpoll;

//=======================================================================================
//  As function for correct initialization ordering
static poll_context & ctx()
{
    static poll_context res;
    return res;
}
//----------------------------------------
//  Can be only as static variable.
static poll_context * first_ctx = & ctx();
//----------------------------------------

//=======================================================================================
void vapplication::_invoke( func_invokable && f )
{
    assert( first_ctx );
    first_ctx->push( std::move(f) );
}
//=======================================================================================
void vapplication::poll()
{
    assert( first_ctx );
    poll_context::poll();
}
//=======================================================================================
void vapplication::stop()
{
    assert( first_ctx );
    first_ctx->stop();
}
//=======================================================================================
