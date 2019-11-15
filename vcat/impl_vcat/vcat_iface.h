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

#ifndef IMPL_VCAT_VCAT_IFACE_H
#define IMPL_VCAT_VCAT_IFACE_H

#include <sstream>
#include <iomanip>
#include <limits>

//=======================================================================================
/*  UPD 2018-07-03  by Elapidae
 *
 *  TODO: сделать нормальное описание принципов работы для разработчиков.
 *  Помогите автору с этим,, чтобы выстроить непротиворечивое описание,
 *  необходимо "объяснить соседу" кухню класса.
 *
 *  Используется CRTP паттерн, наследуемый класс должен определяться ~ так:
 *  class Derived : public vcat_iface<Derived>
 *  {
 *      template<typename T>
 *      void do_cat(T val)
 *      {
 *          // конкатекация с реальным std::ostream;
 *          // сюда могут прилетать, в т.ч., модификаторы.
 *      }
 *  };
 *
 *  В do_cat будут прилетать "очищенные" типы, логика установки флагов и принятия решений
 *  что писать остается в реализациях _cat(...).
 *
 *  UPD: 2019-09-10 by elapidae:
 *   - перенесен в vlibs-2;
 *   - выкинуты ненужные enum _space, _nospace. Сами методы и так работают;
 *   - space()/nospace дополнены delimiter(char)/nodelimiter();
 *
 *  UPD: 2019-09-19 -- перенесен в namespace impl.
 *  UPD: 2019-11-08 -- namespace теперь impl_vcat.
**/
//=======================================================================================


//=======================================================================================
//      impl_vcat::vcat_iface
//=======================================================================================
namespace impl_vcat
{
    template<typename D>
    class vcat_iface
    {
        using _std_hex_type             = decltype( std::hex                );
        using _std_setprecision_type    = decltype( std::setprecision(1)    );
        using _std_setfill_type         = decltype( std::setfill(' ')       );
        using _std_setw_type            = decltype( std::setw(1)            );

    public:
        //-------------------------------------------------------------------------------

        template< typename T >
        D& operator()( T&& val );

        template< typename T, typename ... Ts >
        D& operator()( T&& val, Ts&& ... args );

        template< typename T >
        D& operator << ( T&& val );

        //-------------------------------------------------------------------------------
        //  Сбор модификаторов.

        D& oct();                    // equal to std::oct
        D& dec();                    // equal to std::dec
        D& hex();                    // equal to std::hex

        D& precision( int p );       // equal to std::setprecision(p);
        D& max_precision();

        D& fill_char( char ch );     // equal to std::setfill(ch)
        D& field_width( int w );     // equal to std::setw(fieldWidth)

        D& space();                  // Пробелы между аргументами.
        D& nospace();                // Отключает вывод символа между аргументами.

        D& delimiter(char d);        // Произвольный символ между аргументами.
        D& nodelimiter();            // Отключает вывод символа между аргументами.

        D& num( long long val, int field_width, char fill = ' ' );

        //-------------------------------------------------------------------------------
    protected:

        bool delimiter_was_added() const { return _delimiter_added; }

        vcat_iface();

        template< typename ... Ts >
        explicit vcat_iface( Ts&& ... args );

        ~vcat_iface() = default;

    private:
        char _delimiter;
        bool _with_delimiters = false;
        bool _delimiter_added = false;

        template< typename T >
        D& _cat ( T&& val );

        template< typename T >
        D& _mod_cat ( T && modifier );

        D& _cat ( _std_hex_type modifier          );
        D& _cat ( _std_setprecision_type modifier );
        D& _cat ( _std_setfill_type modifier      );
        D& _cat ( _std_setw_type modifier         );
    };
    //===================================================================================
    //      vcat_iface
    //===================================================================================


    //===================================================================================
    //      Implementation
    //===================================================================================

    //===================================================================================
    template< typename D >
    vcat_iface<D>::vcat_iface()
    {}
    //-----------------------------------------------------------------------------------
    template< typename D >
    template< typename ... Ts >
    vcat_iface<D>::vcat_iface( Ts&& ... args )
        : vcat_iface()
    {
        operator()( std::forward<Ts>(args)... );
    }
    //===================================================================================

    //===================================================================================
    template< typename D >
    template< typename T >
    D& vcat_iface<D>::_cat( T&& val )
    {
        D& d = static_cast<D&>( *this );

        d.do_cat( std::forward<T>(val) );

        if ( _with_delimiters )
            d.do_cat( _delimiter );

        _delimiter_added = _with_delimiters;

        return d;
    }
    //===================================================================================
    template< typename D >
    template< typename T >
    D& vcat_iface<D>::operator()( T&& val )
    {
        return _cat( std::forward<T>(val) );
    }
    //===================================================================================
    template< typename D >
    template< typename T, typename ... Ts >
    D& vcat_iface<D>::operator()( T&& val, Ts&& ... args )
    {
        _cat( std::forward<T>(val) );
        return operator()( std::forward<Ts>(args)... );
    }
    //===================================================================================
    template< typename D >
    template< typename T >
    D& vcat_iface<D>::operator << ( T&& val )
    {
        return _cat( std::forward<T>(val) );
    }
    //===================================================================================

    //===================================================================================
    template< typename D >
    template< typename T >
    D& vcat_iface<D>::_mod_cat ( T&& modifier )
    {
        D& d = static_cast<D&>( *this );
        d.do_cat( std::forward<T>(modifier) );
        return d;
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::_cat ( _std_hex_type modifier )
    {
        return _mod_cat( std::move(modifier) );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::_cat ( _std_setprecision_type modifier )
    {
        return _mod_cat( std::move(modifier) );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::_cat ( _std_setfill_type modifier )
    {
        return _mod_cat( std::move(modifier) );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::_cat ( _std_setw_type modifier )
    {
        return _mod_cat( std::move(modifier) );
    }
    //===================================================================================

    //===================================================================================
    template< typename D >
    D& vcat_iface<D>::oct()
    {
        return _cat( std::oct );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::dec()
    {
        return _cat( std::dec );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::hex()
    {
        return _cat( std::hex );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::precision( int p )
    {
        return _cat( std::setprecision(p) );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::max_precision()
    {
        return precision( std::numeric_limits<long double>::digits10 + 1 );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::fill_char( char ch )
    {
        return _cat( std::setfill(ch) );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::field_width( int w )
    {
        return _cat( std::setw(w) );
    }
    //===================================================================================

    //===================================================================================
    template< typename D >
    D& vcat_iface<D>::delimiter(char d)
    {
        _with_delimiters = true;
        _delimiter = d;
        return static_cast<D&>( *this );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::nodelimiter()
    {
        _with_delimiters = false;
        return static_cast<D&>( *this );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::space()
    {
        return delimiter( ' ' );
    }
    //-----------------------------------------------------------------------------------
    template< typename D >
    D& vcat_iface<D>::nospace()
    {
        return nodelimiter();
    }
    //===================================================================================
    //  2018-06-08 -- проба восстановить справедливость к достаточно важной функции:
    //  выводу выровненных целых чисел.
    template< typename D >
    D& vcat_iface<D>::num( long long val, int f_width, char fill_ch )
    {
        return  fill_char(fill_ch).field_width(f_width)( val );
    }
    //===================================================================================
} // namespace impl_vcat
//=======================================================================================
#endif // IMPL_VCAT_VCAT_IFACE_H
//=======================================================================================
//      impl_vcat::vcat_iface.h
//=======================================================================================
