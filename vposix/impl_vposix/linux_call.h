#ifndef IMPL_VPOSIX_LINUX_CALL_H
#define IMPL_VPOSIX_LINUX_CALL_H

#include "impl_vposix/wrap_errno.h"
#include "vcompiler_traits.h"

//=======================================================================================
/*
 * linux_call сделан по образу Qt макроса EINTR_LOOP, который гоняет в
 * цикле вызов до тех пор, пока либо не будет успешного вызова либо пока ошибка
 * будет EINTR, означающая, что вызов функции был сброшен из-за прерывания.
 * Linux такая linux.
 *
 *  linux_call::no_err(...) --  гоняет до исчезновения ошибки EINTR,
 *                              возвращает что вывалилось.
 *
 *  linux_call::check(...)  --  вызывает no_err(), далее проверяет результат и бросает
 *                              исключение в случае реальной ошибки.
*/
//=======================================================================================


//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct linux_call final
    {
        //-------------------------------------------------------------------------------
        template<typename Fun, typename ... Args>
        static auto no_err( Fun fun, Args ... args ) -> decltype( fun(args...) );
        //-------------------------------------------------------------------------------
        template<typename Fun, typename ... Args>
        static auto check ( Fun fun, Args ... args ) -> decltype( fun(args...) );
        //-------------------------------------------------------------------------------
    };
    //===================================================================================


    //===================================================================================
    //      Implementation
    //===================================================================================
    #pragma GCC diagnostic push
    V_PRAGMA_DIAGNOSTIC_IGNORED_NOEXCEPT_TYPE
    template<typename Fun, typename ... Args>
    auto linux_call::no_err( Fun fun, Args ... args ) -> decltype( fun(args...) )
    {
        decltype( fun(args...) ) res;

        do
        {
            res = fun( args ... );
        }
        while ( res == -1 && ErrNo().need_repeat_last_call() );

        return res;
    }
    //===================================================================================
    template<typename Fun, typename ... Args>
    auto linux_call::check( Fun fun, Args ... args ) -> decltype( fun(args...) )
    {
        auto res = no_err( fun, args... );

        if ( res < 0 )
            ErrNo().do_throw( "linux_call::check" );

        return res;
    }
    #pragma GCC diagnostic pop
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_LINUX_CALL_H
