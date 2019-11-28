#ifndef VTHREAD_H
#define VTHREAD_H

#include <functional>
#include <memory>

#include "vcompiler_traits.h"

//=======================================================================================
/*  2019-11-28  by elapidae     vthread -- врапилка к потокам (точнее, std::future)
 *
 *  ----------------------------------------------------
 *  class vthread
 *  {
 *      vthread( alternate_func = nullptr );
 *
 *      void join();
 *
 *      void invoke( func_invokable );
 *      void invoke( func_invokable, args ... );
 *      void invoke( Class*, &Class::method, args ... );
 *  };
 *  ----------------------------------------------------
 *
 *  Запускает поток автоматически.
 *
 *  Метод join() ожидает завершение потока. Вызывается в деструкторе.
 *  NB! Если в потоке случится исключение, то оно будет переброшено в этот метод.
 *
 *
 *  Используется механизм поллинга vpoll.
 *  Все созданные в этом потоке компоненты, которые регистрируются на real_poll,
 *  будут поллиться в этом потоке.
 *
 *  Можно запускать свою функцию в поток (см. конструктор),
 *  при этом можете поллить -- vpoll::poll().
 *
 *  Но предполагается, что основное применение потока -- отправлять в него задачи
 *  через методы invoke:
 *
 *  основной метод:
 *  invoke(func_invokable), например:
 *      thread.invoke( []{ vdeb << "Hello world in own thread."; } );
 *      thread.invoke( some_func_ptr );
 *
 *  дополнительный для классов:
 *  invoke( Class*, ClassMethod, args... ), например:
 *      std::vector<int> v;
 *      thread.invoke( &v, &std::vector<int>::push_back, 42 );
 *
 *  дополнительный для функций:
 *  invoke( Function, args... ), например:
 *      void foo(int);
 *      thread.invoke( foo, 42 );
*/
//=======================================================================================


//=======================================================================================
class vthread final
{
public:
    //-----------------------------------------------------------------------------------
    using func_invokable = std::function<void()>;

    explicit vthread( func_invokable alternate_func = nullptr );
    ~vthread() noexcept(false);

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

    //-----------------------------------------------------------------------------------
private:
    friend struct in_thread_ctl;
    class _pimpl;
    std::unique_ptr<_pimpl> _p;

    static void _run( _pimpl *p );
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
