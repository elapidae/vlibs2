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

#ifndef VCAT_IMPL_CONTAINERS_H
#define VCAT_IMPL_CONTAINERS_H

#include <type_traits>
#include <ostream>
#include <iterator>

#include "vvoid_type.h"


//=======================================================================================
namespace impl
{
    template<typename C> constexpr
    bool is_container_not_string();
}
//=======================================================================================
namespace std
{
    template<typename C> typename std::enable_if<impl::is_container_not_string<C>(),
    std::ostream&>::type operator <<( std::ostream& os, const C& container );
}
//=======================================================================================


//=======================================================================================
//      Implementation
//=======================================================================================
namespace std
{
    //===================================================================================
    template<typename C> typename std::enable_if<impl::is_container_not_string<C>(),
    std::ostream&>::type operator <<( std::ostream& os, const C& container )
    {
        os << "[";

        if ( !container.empty() )
        {
            os << *container.begin();

            // cannot use begin() + 1 because of list...
            auto it = container.begin();
            for ( ++it; it != container.end(); ++it )
                os << ',' << *it;
        }
        os << ']';
        return os;
    }
    //===================================================================================
}
//=======================================================================================

#ifdef V_HAS_QT
    class QString;
    class QByteArray;
#endif // has qt

//=======================================================================================
namespace impl
{
    //===================================================================================
    //      is_container<C>()
    //===================================================================================
    template<typename C, typename = void>
    struct _is_container
        : public std::false_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C>
    struct _is_container
    <
        C,
        vvoid_type
        <
            decltype( std::declval<C>().begin() ),
            decltype( std::declval<C>().end()   ),
            decltype( std::declval<C>().size()  ),
            typename C::value_type
        >
    >
        : public std::true_type
    {};
    //-----------------------------------------------------------------------------------
    template<typename C> constexpr
    bool is_container_not_string()
    {
        return _is_container<C>::value
                && !std::is_base_of<std::string,C>::value
                && !std::is_base_of<std::wstring,C>::value
                #ifdef V_HAS_QT
                    && !std::is_base_of<QString,C>::value
                    && !std::is_base_of<QByteArray,C>::value
                #endif // has qt
               ;
    }
    //===================================================================================
} // namespace impl
//=======================================================================================


#endif // VCAT_IMPL_CONTAINERS_H
