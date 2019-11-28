#include "wrap_sys_timerfd.h"

#include <cassert>
#include <sys/timerfd.h>
#include "impl_vposix/linux_call.h"
#include "impl_vposix/wrap_errno.h"
#include "impl_vposix/wrap_unistd.h"

using namespace impl_vposix;

//=======================================================================================
static void set_timer( int fd,
                       time_t val_sec, long val_nanosec,
                       time_t int_sec, long int_nanosec )
{
    itimerspec spec;

    spec.it_value.tv_sec  = val_sec;
    spec.it_value.tv_nsec = val_nanosec;

    spec.it_interval.tv_sec  = int_sec;
    spec.it_interval.tv_nsec = int_nanosec;

    int flags = 0; // May be TFD_TIMER_ABSTIME

    linux_call::check( ::timerfd_settime, fd, flags, &spec, nullptr );
}
//=======================================================================================

//=======================================================================================
int wrap_sys_timerfd::create()
{
    int id = CLOCK_MONOTONIC;
    int flags = TFD_NONBLOCK | TFD_CLOEXEC;

    return linux_call::check( ::timerfd_create, id, flags );
}
//=======================================================================================
void wrap_sys_timerfd::start( int fd, time_t sec, long nanosec )
{
    set_timer( fd, sec, nanosec, sec, nanosec );
}
//=======================================================================================
void wrap_sys_timerfd::singleshot( int fd, time_t sec, long nanosec )
{
    set_timer( fd, sec, nanosec, 0, 0 );
}
//=======================================================================================
void wrap_sys_timerfd::stop( int fd )
{
    set_timer( fd, 0, 0, 0, 0 );
}
//=======================================================================================
u_int64_t wrap_sys_timerfd::read( int fd )
{
    uint64_t res = 0;
    auto sz = wrap_unistd::read_no_err( fd, &res, sizeof(res) );
    if ( sz == -1 )
    {
        ErrNo err;
        if ( !err.resource_unavailable_try_again() )
            err.do_throw( "wrap_sys_timerfd::read" );
    }
    assert( sz == sizeof(res) );

    return res;
}
//=======================================================================================
