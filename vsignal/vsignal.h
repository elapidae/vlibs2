/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  Authors:
**  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
**  Nadezhda Churikova aka claorisel    claorisel@gmail.com
**  Ekaterina Boltenkova aka kataretta  kitkat52@yandex.ru
**  Ivan Deylid aka sid1057             ivanov.dale@gmail.com>
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifndef VSIGNAL_H
#define VSIGNAL_H


#include <functional>
#include <map>
#include <stdexcept>

//=======================================================================================
/*
 *      VSignal -- некоторый аналог сигналов в Qt. Используется для обеспечения
 *      модульности программ.
 *
 *      Принцип использования:
 *      - Объявляем сигнал с сигнатурой вызова в классе:
 *          class AnyEmitter
 *          {
 *          public:
 *              VSignal<int, std::string> some_signal;
 *
 *              void emit_signal()
 *              {
 *                  some_signal( 42, "Hello world!" );
 *              }
 *          };
 *
 *      - Из внешнего кода соединяемся с сигналом:
 *          class Receiver
 *          {
 *          public:
 *              void on_some_signal( int a, const std::string &b );
 *          };
 *          AnyEmitter emitter;
 *          Receiver receiver;
 *          emitter.some_signal.link( &receiver, &Receiver::on_some_signal );
 *
 *      - Вызываем приемник:
 *          emitter.emit_signal();
 *
 *      - Можно подключать лямбды и функции:
 *          emitter.some_signal.link( [](int a, std::string b) {
 *              std::cout << "a = " << a << ", b = " << b << std::endl;
 *          });
 *          void some_func(int a, const std::string &b) {}
 *          emitter.some_signal.connect( &some_func );
 *
 *      - Можно использовать += и -= для функций.
*/
//=======================================================================================


//=======================================================================================
//      VSignal
//=======================================================================================
template< typename ... Args >
class VSignal
{
public:
    class link_id;

    VSignal();

    template< typename Fn >
    link_id link( Fn&& fn );

    template< typename Cls, typename Fn >
    link_id link( Cls *cls, Fn fn );

    template< typename Fn >
    link_id operator += ( Fn&& fn );

    void unlink( link_id id );
    void operator -= ( link_id id );

    void call( const Args& ... args );
    void operator()( const Args& ... args );

private:
    // do not use this connector.
    void link( VSignal<Args...> repeater ) = delete;

    using Func = std::function< void(const Args&...) >;

    std::map<int,Func> _funcs;

    int _cur_id = 0;

    VSignal( const VSignal& ) = delete;
    const VSignal& operator = ( const VSignal& ) = delete;
};
//=======================================================================================
//      VSignal
//=======================================================================================



//=======================================================================================
//      IMPLEMENTATION
//=======================================================================================
template< typename ... Args >
class VSignal<Args...>::link_id
{
public:
    link_id() {}

private:
    friend class VSignal<Args...>;

    link_id( int id )
        : _id(id)
    {}

    int _id = -1;
};
//=======================================================================================
template< typename ... Args >
VSignal<Args...>::VSignal()
{}
//=======================================================================================
template< typename ... Args >
template< typename Fn >
typename VSignal<Args...>::link_id VSignal<Args...>::link( Fn&& fn )
{
    _funcs.insert( {++_cur_id, Func(std::forward<Fn>(fn))} );
    return _cur_id;
}
//=======================================================================================
template< typename ... Args >
template< typename Fn >
typename VSignal<Args...>::link_id VSignal<Args...>::operator+=( Fn&& fn )
{
    _funcs.insert( {++_cur_id, std::forward<Fn>(fn)} );
    return _cur_id;
}
//=======================================================================================
template< typename ... Args >
template< typename Cls, typename Fn >
typename VSignal<Args...>::link_id VSignal<Args...>::link( Cls *cls, Fn fn )
{
    return link( [cls,fn](const Args& ... args){ (cls->*fn)(args...); } );
}
//=======================================================================================
template< typename ... Args >
void VSignal<Args...>::unlink( link_id id )
{
    auto count = _funcs.erase( id._id );
    if ( count == 0 )
        throw std::runtime_error( "Cannot unlink VSignal::link_id." );
}
//=======================================================================================
template< typename ... Args >
void VSignal<Args...>::operator-=( link_id id )
{
    unlink( id );
}
//=======================================================================================
template< typename ... Args >
void VSignal<Args...>::call( const Args& ... args )
{
    for( const auto& f: _funcs )
    {
        f.second( args... );
    }
}
//=======================================================================================
template< typename ... Args >
void VSignal<Args...>::operator()( const Args& ... args )
{
    call( args... );
}
//=======================================================================================

#endif // VSIGNAL_H
