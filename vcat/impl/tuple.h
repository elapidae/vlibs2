#ifndef IMPL_TUPLE_H
#define IMPL_TUPLE_H

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
namespace impl
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
} // impl namespace
//=======================================================================================

//=======================================================================================
namespace std
{
    //===================================================================================
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wunused-function"
    static ostream& operator <<( ostream& os, const std::tuple<>& )
    {
        os << "{}";
        return os;
    }
    #pragma GCC diagnostic pop
    //===================================================================================
    template<typename T, typename ... Args>
    ostream& operator <<( ostream& os, const std::tuple<T,Args...> &tup )
    {
        os << "{";
        constexpr size_t sz = tuple_size< tuple<T,Args...> >();
        impl::_print<0,sz,sz != 0>(os, tup);
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

#endif // IMPL_TUPLE_H
