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


#ifndef IMPL_VCAT_TUPLE_H
#define IMPL_VCAT_TUPLE_H

#include <tuple>
#include <ostream>

//=======================================================================================
namespace std
{
    //===================================================================================
    static ostream& operator << ( ostream& os, const std::tuple<>& );
    //===================================================================================
    template<typename T, typename ... Args>
    ostream& operator << ( ostream& os, const std::tuple<T,Args...> &tup );
    //===================================================================================
    // pair is not same as tuple<T1,T2> :(((
    template<typename T, typename U>
    ostream& operator << ( ostream& os, const std::pair<T,U> &p );
    //===================================================================================
} // std namespace
//=======================================================================================



//=======================================================================================
//      Implementation
//=======================================================================================
namespace impl_vcat
{
    template<size_t pos, size_t endpos>
    void _print( std::ostream& os, const std::tuple<>& )
    {
        os << "}";
    }
    //===================================================================================
    template<size_t pos, size_t endpos, bool process, typename T, typename ... Args>
    void _print( std::ostream& os, const std::tuple<T,Args...> &tup )
    {
        if ( !process )
        {
            os << "}";
            return;
        }
        os << std::get<pos>(tup);
        if ( pos + 1 < endpos  )
            os << ",";

        _print< pos + 1 < endpos ? pos + 1 : pos,
                endpos,
                pos + 1 < endpos >(os,tup);
    }
} // impl_vcat namespace
//=======================================================================================

//=======================================================================================
namespace std
{
    //===================================================================================
    static ostream& operator <<( ostream& os, const std::tuple<>& )
    {
        // Сиё безобразие чтобы старые компиляторы не мешали ворнингами.
        using _not_use_ = ostream&(&)(ostream&, const std::tuple<>&);
        constexpr _not_use_ _val_ = static_cast<_not_use_>(operator<<);
        (void)(_val_);

        os << "{}";
        return os;
    }
    //===================================================================================
    template<typename T, typename ... Args>
    ostream& operator <<( ostream& os, const std::tuple<T,Args...> &tup )
    {
        os << "{";
        constexpr size_t sz = tuple_size< tuple<T,Args...> >();
        impl_vcat::_print<0,sz,sz != 0>(os, tup);
        return os;
    }
    //===================================================================================
    template<typename T, typename U>
    ostream& operator <<( ostream& os, const std::pair<T,U> &p )
    {
        os << '{' << p.first << ',' << p.second << '}';
        return os;
    }
    //===================================================================================
} // std namespace
//=======================================================================================

#endif // IMPL_VCAT_TUPLE_H
