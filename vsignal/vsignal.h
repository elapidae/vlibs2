/****************************************************************************************
**
**  VLIBS codebase, NIIAS
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
 *      vsignal -- некоторый аналог сигналов в Qt. Используется для обеспечения
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
//      vsignal
//=======================================================================================
template< typename ... Args >
class vsignal
{
public:
    class link_id;

    vsignal();

    template< typename Fn >
    link_id link( Fn&& fn );

    template< typename Cls, typename Fn >
    link_id link( Cls *cls, Fn fn );

    template< typename Fn >
    link_id operator += ( Fn&& fn );

    void unlink_all();
    void unlink( link_id id );
    void operator -= ( link_id id );

    void call( const Args& ... args );
    void operator()( const Args& ... args );

private:
    // do not use this connector.
    void link( vsignal<Args...> repeater ) = delete;

    using Func = std::function< void(const Args&...) >;

    std::map<int,Func> _funcs;

    int _cur_id = 0;

    vsignal( const vsignal& ) = delete;
    const vsignal& operator = ( const vsignal& ) = delete;
};
//=======================================================================================
//      VSignal
//=======================================================================================


//=======================================================================================
//      IMPLEMENTATION
//=======================================================================================
template< typename ... Args >
class vsignal<Args...>::link_id
{
public:
    link_id() {}

private:
    friend class vsignal<Args...>;

    link_id( int id, vsignal<Args...> * owner )
        : _id    ( id    )
        , _owner ( owner )
    {}

    int _id = -1;
    vsignal<Args...> * _owner = nullptr;
};
//=======================================================================================
template< typename ... Args >
vsignal<Args...>::vsignal()
{}
//=======================================================================================
template< typename ... Args >
template< typename Fn >
typename vsignal<Args...>::link_id vsignal<Args...>::link( Fn&& fn )
{
    _funcs.insert( {++_cur_id, Func(std::forward<Fn>(fn))} );
    return { _cur_id, this };
}
//=======================================================================================
template< typename ... Args >
template< typename Fn >
typename vsignal<Args...>::link_id vsignal<Args...>::operator+=( Fn&& fn )
{
    return link( std::forward<Fn>(fn) );
}
//=======================================================================================
template< typename ... Args >
template< typename Cls, typename Fn >
typename vsignal<Args...>::link_id vsignal<Args...>::link( Cls *cls, Fn fn )
{
    return link( [cls,fn](const Args& ... args){ (cls->*fn)(args...); } );
}
//=======================================================================================
template< typename ... Args >
void vsignal<Args...>::unlink_all()
{
    _funcs.clear();
}
//=======================================================================================
template< typename ... Args >
void vsignal<Args...>::unlink( link_id id )
{
    if ( id._owner != this )
        throw std::runtime_error( "VSignal::unlink(id): signal not an owner." );

    auto count = _funcs.erase( id._id );
    if ( count == 0 )
        throw std::runtime_error( "VSignal::unlink(id): link has been lost." );
}
//=======================================================================================
template< typename ... Args >
void vsignal<Args...>::operator-=( link_id id )
{
    unlink( id );
}
//=======================================================================================
template< typename ... Args >
void vsignal<Args...>::call( const Args& ... args )
{
    for( const auto& f: _funcs )
    {
        f.second( args... );
    }
}
//=======================================================================================
template< typename ... Args >
void vsignal<Args...>::operator()( const Args& ... args )
{
    call( args... );
}
//=======================================================================================

#endif // VSIGNAL_H
