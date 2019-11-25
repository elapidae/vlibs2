#ifndef IMPL_VPOSIX_LINUX_CALL_H
#define IMPL_VPOSIX_LINUX_CALL_H

#include "impl_vposix/verrno.h"

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class linux_call
    {
        //===============================================================================
        template<typename Fun, typename ... Args>
        static auto no_err( Fun fun, Args ... args) -> decltype( fun(args...) );
        //===============================================================================
        template<typename Fun, typename ... Args>
        static auto check( Fun fun, Args ... args) -> decltype( fun(args...) );
        //===============================================================================
    };
    //===================================================================================


    //===================================================================================
    //      Implementation
    //===================================================================================
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wnoexcept-type"
    template<typename Fun, typename ... Args>
    auto linux_call::no_err( Fun fun, Args ... args) -> decltype( fun(args...) )
    {
        decltype( fun(args...) ) res;

        do
        {
            res = fun( args ... );
        }
        while ( res == -1 && VErrNo().need_repeat_last_call() );

        return res;
    }
    #pragma GCC diagnostic pop
    //===================================================================================
    template<typename Fun, typename ... Args>
    auto linux_call::check( Fun fun, Args ... args) -> decltype( fun(args...) )
    {
        auto res = call_no_err( fun, args... );

        if ( res < 0 )
            VErrNo().throw_if_has();

        return res;
    }
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_LINUX_CALL_H
