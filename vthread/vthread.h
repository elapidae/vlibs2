#ifndef VTHREAD_H
#define VTHREAD_H

#include <functional>
#include <memory>

#include "vcompiler_traits.h"

//=======================================================================================
class vthread final
{
public:
    using func_invokable = std::function<void()>;

    vthread();
    ~vthread();

    void join();

    void invoke( func_invokable );

    #if V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS
        template< typename Fn, typename A1, typename ... Args >
        void invoke( Fn fn, A1 a1, Args ... args );

        template< typename Cls, typename Fn, typename ... Args >
        void invoke( Cls* cls, Fn fn, Args ... args );
    #else
        template< typename Fn, typename A1 >
        void invoke( Fn fn, A1 a1 );
        template< typename Fn, typename A1, typename A2 >
        void invoke( Fn fn, A1 a1, A2 a2 );

        template< typename Cls, typename Fn >
        void invoke( Cls* cls, Fn fn );
        template< typename Cls, typename Fn, typename A1 >
        void invoke( Cls* cls, Fn fn, A1 a1 );
        template< typename Cls, typename Fn, typename A1, typename A2 >
        void invoke( Cls* cls, Fn fn, A1 a1, A2 a2 );
    #endif

private:
    friend struct in_thread_ctl;
    class _pimpl;
    std::unique_ptr<_pimpl> _p;

    static void _run( vthread *self );
    void _invoke( func_invokable&& );
};
//=======================================================================================
//      Implementation
//=======================================================================================
#if V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS
    //===================================================================================
    template< typename Fn, typename A1, typename ... Args >
    void vthread::invoke( Fn fn, A1 a1, Args ... args )
    {
        invoke( [=]{ fn(a1, args...); } );
    }
    //===================================================================================
    template< typename Cls, typename Fn, typename ... Args >
    void vthread::invoke( Cls* cls, Fn fn, Args ... args )
    {
        invoke( [=]{ (cls->*fn)(args...); } );
    }
    //===================================================================================
#else
    //===================================================================================
    template< typename Fn, typename A1 >
    void vthread::invoke( Fn fn, A1 a1 )
    {
        invoke( [=]{ fn(a1); } );
    }
    //-----------------------------------------------------------------------------------
    template< typename Fn, typename A1, typename A2 >
    void vthread::invoke( Fn fn, A1 a1, A2 a2 )
    {
        invoke( [=]{ fn(a1,a2); } );
    }
    //===================================================================================
    template< typename Cls, typename Fn >
    void vthread::invoke( Cls* cls, Fn fn )
    {
        invoke( [=]{ (cls->*fn)(); } );
    }
    //-----------------------------------------------------------------------------------
    template< typename Cls, typename Fn, typename A1 >
    void vthread::invoke( Cls* cls, Fn fn, A1 a1 )
    {
        invoke( [=]{ (cls->*fn)(a1); } );
    }
    //-----------------------------------------------------------------------------------
    template< typename Cls, typename Fn, typename A1, typename A2 >
    void vthread::invoke( Cls* cls, Fn fn, A1 a1, A2 a2 )
    {
        invoke( [=]{ (cls->*fn)(a1,a2); } );
    }
    //===================================================================================
#endif
//=======================================================================================

#endif // VTHREAD_H
