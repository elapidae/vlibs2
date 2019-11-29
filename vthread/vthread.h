#ifndef VTHREAD_H
#define VTHREAD_H

#include <functional>
#include <memory>

#include "vinvoke_iface.h"

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
//      vapplication_thread
//=======================================================================================
//class _vapplication_thread final : public vinvoke_iface
//{
//private:
//    _vapplication_thread();
//    virtual void _invoke( func_invokable&& ) override;
//};
//extern _vapplication_thread vapplication;
//=======================================================================================
//      vapplication_thread
//=======================================================================================


//=======================================================================================
//      vthread
//=======================================================================================
class vthread final : public vinvoke_iface
{
public:
    //-----------------------------------------------------------------------------------
    using func_invokable = std::function<void()>;

    explicit vthread( func_invokable alternate_func = nullptr );
    ~vthread() noexcept(false);

    void join();

    //-----------------------------------------------------------------------------------
private:
    friend struct in_thread_ctl;
    class _pimpl;
    std::unique_ptr<_pimpl> _p;

    static void _run( _pimpl *p );

    virtual void _invoke( func_invokable&& ) override;
};
//=======================================================================================
//      vthread
//=======================================================================================


#endif // VTHREAD_H
