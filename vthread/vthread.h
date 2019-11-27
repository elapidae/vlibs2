#ifndef VTHREAD_H
#define VTHREAD_H

#include <functional>
#include <memory>


//=======================================================================================
class vthread final
{
public:
    using func_invokable = std::function<void()>;

    vthread();
    ~vthread();

    void join();

    void invoke( func_invokable );

    template< typename Cls, typename Fn, typename ... Args >
    void invoke( Cls* cls, Fn fn, Args&& ... args );

private:
    friend struct in_thread_ctl;
    class _pimpl;
    std::unique_ptr<_pimpl> _p;

    static void _run( vthread *self );
    void _invoke( func_invokable&& );
};
//=======================================================================================
template< typename Cls, typename Fn, typename ... Args >
void vthread::invoke( Cls* cls, Fn fn, Args&& ... args )
{
    invoke( [=](){ (cls->*fn)(std::forward<Args>(args)...); } );
}
//=======================================================================================

#endif // VTHREAD_H
