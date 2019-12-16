#ifndef VINVOKE_IFACE_H
#define VINVOKE_IFACE_H

#include "vcompiler_traits.h"
#include <functional>

//=======================================================================================
class vinvoke_iface
{
public:
    //-----------------------------------------------------------------------------------

    using task_type = std::function<void()>;

    void invoke( task_type f );

    virtual size_t tasks_count() const = 0;
    virtual void   tasks_clear() = 0;

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

    //-----------------------------------------------------------------------------------
protected:
    virtual void _invoke( task_type&& ) = 0;
    ~vinvoke_iface(); // not virtual, all ok.
};
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
#if V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS
    //===================================================================================
    template< typename Fn, typename A1, typename ... Args >
    void vinvoke_iface::invoke( Fn fn, A1 a1, Args ... args )
    {
        invoke( [=]{ fn(a1, args...); } );
    }
    //===================================================================================
    template< typename Cls, typename Fn, typename ... Args >
    void vinvoke_iface::invoke( Cls* cls, Fn fn, Args ... args )
    {
        invoke( [=]{ (cls->*fn)(args...); } );
    }
    //===================================================================================
#else
    //===================================================================================
    template< typename Fn, typename A1 >
    void vinvoke_iface::invoke( Fn fn, A1 a1 )
    {
        invoke( [=]{ fn(a1); } );
    }
    //-----------------------------------------------------------------------------------
    template< typename Fn, typename A1, typename A2 >
    void vinvoke_iface::invoke( Fn fn, A1 a1, A2 a2 )
    {
        invoke( [=]{ fn(a1,a2); } );
    }
    //===================================================================================
    template< typename Cls, typename Fn >
    void vinvoke_iface::invoke( Cls* cls, Fn fn )
    {
        invoke( [=]{ (cls->*fn)(); } );
    }
    //-----------------------------------------------------------------------------------
    template< typename Cls, typename Fn, typename A1 >
    void vinvoke_iface::invoke( Cls* cls, Fn fn, A1 a1 )
    {
        invoke( [=]{ (cls->*fn)(a1); } );
    }
    //-----------------------------------------------------------------------------------
    template< typename Cls, typename Fn, typename A1, typename A2 >
    void vinvoke_iface::invoke( Cls* cls, Fn fn, A1 a1, A2 a2 )
    {
        invoke( [=]{ (cls->*fn)(a1,a2); } );
    }
    //===================================================================================
#endif
//=======================================================================================

#endif // VINVOKE_IFACE_H
