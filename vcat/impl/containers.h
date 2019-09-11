#ifndef IMPL_CONTAINERS_H
#define IMPL_CONTAINERS_H

#include <type_traits>
#include <ostream>
#include <iterator>
//#include "impl/tuple.h"

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

//=======================================================================================
namespace impl
{
    //===================================================================================
    //  I know, that where is std::void_t. It is not work with GCC 4.7.3...
    template <typename ... Ts>
    using void_type = void;
    //===================================================================================

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
        void_type
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
        return _is_container<C>::value &&
               !std::is_base_of<std::string,C>::value;
    }
    //===================================================================================
} // namespace impl
//=======================================================================================


#endif // IMPL_CONTAINERS_H
