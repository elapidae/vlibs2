#ifndef VVOID_TYPE_H
#define VVOID_TYPE_H

#include <type_traits>
#include "vcompiler_traits.h"

//=======================================================================================
#ifdef V_DO_NOT_KNOW_STD_VOID_T
    //===================================================================================
    namespace impl
    {
        template<class C0>
        constexpr void _define_void_type_()
        {
            using sss = C0;
        }

        template< class C0, class C1, class... Cls >
        constexpr void _define_void_type_()
        {
            using sss = C0;
            _define_void_type_<C1,Cls...>();
        }
    } // namespace impl

    template< class... Cls >
    using vvoid_type = decltype( impl::_define_void_type_<Cls...>() );
    //===================================================================================
#else // V_DO_NOT_KNOW_STD_VOID_T
    //===================================================================================
    template <class ... Cs>
    using vvoid_type = std::void_t<Cs...>;
    //===================================================================================
#endif // V_DO_NOT_KNOW_STD_VOID_T


#endif // VVOID_TYPE_H
