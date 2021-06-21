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

//=======================================================================================
//      vcat.h
//=======================================================================================
#ifndef VCAT_H
#define VCAT_H

#include <stdexcept>

#include "vcompiler_traits.h"
#include "impl_vcat/vcat_iface.h"

//=======================================================================================
/*      2018-03-20       VCAT
 *  UPD 2018-07-03
 *
 *  Класс предназначен для составления сообщений в строку.
 *  Имя берет по аналогии с программой cat, делает, в принципе, то же.
 *
 *  Принцип работы: можете использовать его просто перечисляя нужные поля подряд:
 *  vcat("one", 2, "three").str();
 *
 *  Метод str() позволяет явно получить результирующую строку.
 *
 *  Можно ставить друг за другом произвольное количество скобок:
 *  vcat("one", 2, "three")(4.0, 5.1f);
 *
 *  Можно пользовать как стандартный поток:
 *  vcat cat;
 *  cat << "one" << "two" << std::setw(10) << 3;
 *
 *  Действуют все /проверенные/ модификаторы потоков. Часть из них перенесена в методы
 *  класса (чтобы были под рукой). Вообще, модификаторов как грязи, притом каждый себя
 *  ведет как хочет, ниже приведены ссылки на документацию по части модификаторов.
 *
 *  Есть методы delimiter(char), nodelimiter(), включающий вставку произвольного символа
 *  между аргументами. Методы space(), nospace(), включают|отключают вставку пробелов,
 *  то же самое что и delimiter(' ').
 *
 *  Если будет непонятно или неудобно, прошу сообщить, помогите сделать описание и
 *  код лучше. Если поможете с юнит-тестированием -- цены вам не будет!
 *
 *  Сделан статический метод from_text<T>( string ) для преобразования значений из
 *  строк через стандартные механизмы (istringstream). В последствии, надо будет
 *  перенести в модуль vstring. NB! Если поток сломается, бросает исключение
 *  std::runtime_error.
 *
 *
 *  Вопросы, пожелания, предложения, просьбы добавить к/л модификатор или
 *  его эффект -- *WELCOME!*
 *
*/
//=======================================================================================
//  Манипуляторы в интернете:
//http://en.cppreference.com/w/cpp/io/manip         -- list of manips.
//http://en.cppreference.com/w/cpp/io/manip/setw
//http://en.cppreference.com/w/cpp/io/manip/setfill
//http://en.cppreference.com/w/cpp/io/manip/put_money
//http://en.cppreference.com/w/cpp/io/manip/put_time
//http://en.cppreference.com/w/cpp/io/manip/skipws
//http://en.cppreference.com/w/cpp/io/manip/ws
//http://en.cppreference.com/w/cpp/io/manip/ends
//http://en.cppreference.com/w/cpp/io/manip/showpos
//http://en.cppreference.com/w/cpp/io/manip/boolalpha
//http://en.cppreference.com/w/cpp/io/manip/showbase
//http://en.cppreference.com/w/cpp/io/manip/setprecision
//http://en.cppreference.com/w/cpp/io/manip/showpoint
//=======================================================================================



//=======================================================================================
//      vcat -- составитель строк - сообщений методами STL (ostringstream)
//=======================================================================================
class vcat : public impl_vcat::vcat_iface<vcat>
{
public:
    //-----------------------------------------------------------------------------------

    explicit vcat();

    template< typename ... Ts >
    explicit vcat( Ts&& ... args );

    std::string str()       const;
    operator std::string()  const;

    //-----------------------------------------------------------------------------------

    template< typename T >
    static T from_text( const std::string& text );

    //-----------------------------------------------------------------------------------
private:
    void _init_default_modifiers();
    std::ostringstream _stream;

    friend class impl_vcat::vcat_iface<vcat>;
    template<typename T> void do_cat( T&& val );

    vcat( vcat&& )                  = delete;
    vcat( const vcat& )             = delete;
    vcat& operator= ( vcat&& )      = delete;
    vcat& operator= ( const vcat& ) = delete;
};
//=======================================================================================
//      vcat
//=======================================================================================


//=======================================================================================
// Для того, чтобы vcat можно было выводить без указания .str();
std::ostream& operator << ( std::ostream& s, const vcat& c );
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
template< typename ... Ts >
vcat::vcat( Ts&& ... args )
{
    _init_default_modifiers();
    operator()( std::forward<Ts>(args)... );
}
//=======================================================================================
template<typename T>
void vcat::do_cat( T&& val )
{
    _stream << std::forward<T>(val);
}
//=======================================================================================


//=======================================================================================
//  TODO: Переехать в vstring.
template< typename T >
T vcat::from_text( const std::string& text )
{
    std::istringstream ss( text );
    T res;
    ss >> res;

    if ( ss.fail() || ss.bad() )
        throw std::runtime_error( std::string("Error during interpretation text: '") +
                text + "', in function " + V_PRETTY_FUNCTION );

    return res;
}
//=======================================================================================


#endif // VCAT_H
//=======================================================================================
//      /vcat.h
//=======================================================================================
