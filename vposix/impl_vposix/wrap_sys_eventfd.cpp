#include "wrap_sys_eventfd.h"

#include <cassert>
#include <sys/eventfd.h>
#include "impl_vposix/linux_call.h"
#include "impl_vposix/wrap_errno.h"
#include "impl_vposix/wrap_unistd.h"

using namespace impl_vposix;



//=======================================================================================
//      eventfd as normal class
//=======================================================================================
eventfd::eventfd()
    : _evfd( wrap_sys_eventfd::create() )
{}
//=======================================================================================
eventfd::~eventfd()
{
    wrap_unistd::close( _evfd );
}
//=======================================================================================
bool eventfd::read()
{
    return wrap_sys_eventfd::read( _evfd );
}
//=======================================================================================
void eventfd::write()
{
    wrap_sys_eventfd::write( _evfd );
}
//=======================================================================================
int eventfd::handle() const
{
    return _evfd;
}
//=======================================================================================

//=======================================================================================
//      wrap_sys_eventfd, only wrappers for kernel calls
//=======================================================================================
int impl_vposix::wrap_sys_eventfd::create()
{
    int flags = EFD_CLOEXEC | EFD_NONBLOCK | EFD_SEMAPHORE;
    unsigned int count = 0; // initial count of events.

    return linux_call::check( ::eventfd, count, flags );
}
//=======================================================================================
bool wrap_sys_eventfd::read( int evfd )
{
    eventfd_t buf;
    auto res = linux_call::no_err( ::eventfd_read, evfd, &buf );
    if ( res == 0 )
        return true;

    ErrNo e;
    if ( e.resource_unavailable_try_again() )
        return false;

    e.do_throw( "wrap_sys_eventfd::read" );

    assert( buf == 1 );
    return true;
}
//=======================================================================================
void wrap_sys_eventfd::write( int evfd )
{
    linux_call::check( ::eventfd_write, evfd, 1 );
}
//=======================================================================================
