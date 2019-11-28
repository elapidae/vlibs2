#ifndef IMPL_VPOSIX_WRAP_SYS_TIMERFD_H
#define IMPL_VPOSIX_WRAP_SYS_TIMERFD_H

//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man2/timerfd_create.2.html
*/
//=======================================================================================

#include <sys/types.h>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_sys_timerfd final
    {
        //-------------------------------------------------------------------------------
        static int       create();

        static void      start       ( int fd, time_t sec, long nanosec );
        static void      singleshot  ( int fd, time_t sec, long nanosec );

        static void      stop( int fd );

        static u_int64_t read( int fd );
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_SYS_TIMERFD_H
