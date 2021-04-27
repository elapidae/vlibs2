#ifndef VENDIAN_H
#define VENDIAN_H

#include <stdint.h>
#include <stdexcept>
#include <type_traits>

//=======================================================================================

#include "details/vplatform.h"

#ifdef V_COMPILER_GNU
    #define V_ENDIAN_USE_SYSTEM_ENDIAN_H
#elif ( defined(V_COMPILER_MINGW) || defined(V_COMPILER_MSVC) )
    #define V_ENDIAN_USE_OWN_REALIZATION
#else
    #error "Cannot define compiler for vendian functional."
#endif

//=======================================================================================

#ifdef V_ENDIAN_USE_SYSTEM_ENDIAN_H
    #include <endian.h>
#endif

//=======================================================================================
namespace v
{
    namespace endian
    {
        //===============================================================================
        #if defined ( V_ENDIAN_USE_SYSTEM_ENDIAN_H )

            static constexpr bool is_little()
            {
                return BYTE_ORDER == LITTLE_ENDIAN;
            }

            static constexpr bool is_big()
            {
                return BYTE_ORDER == BIG_ENDIAN;
            }
        //-------------------------------------------------------------------------------
        #elif defined( V_ENDIAN_USE_OWN_REALIZATION )

            static bool is_little() noexcept
            {
                static const uint32_t u32 = 0x12345678;
                static const uint8_t* ptr =
                        static_cast<const uint8_t*>(static_cast<const void*>(&u32));

                return
                    ptr[0] == 0x78 &&
                    ptr[1] == 0x56 &&
                    ptr[2] == 0x34 &&
                    ptr[3] == 0x12;
            }

            static bool is_big() noexcept
            {
                static const uint32_t u32 = 0x12345678;
                static const uint8_t* ptr =
                        static_cast<const uint8_t*>(static_cast<const void*>(&u32));

                return
                    ptr[0] == 0x12 &&
                    ptr[1] == 0x34 &&
                    ptr[2] == 0x56 &&
                    ptr[3] == 0x78;
            }
        //-------------------------------------------------------------------------------
        #else
            #error "Cannot understand how to realize endian."
        #endif
        //-------------------------------------------------------------------------------
        static bool endian_not_defined()
        {
            return !is_big() && !is_little();
        }

        static bool _abort_on_error = endian_not_defined()
                ? (throw std::logic_error("Cannot detect endian :((("), true)
                : false;

        //===============================================================================

        template <typename T> std::enable_if_t<std::is_arithmetic_v<T>, T>
        static reverse_val( T val ) noexcept
        {
            static_assert ( sizeof(T) <= 8, "Cannot reverse values more than 8 bytes." );

            constexpr auto sz = sizeof(val);
            uint8_t* ptr = static_cast<uint8_t*>( static_cast<void*>(&val) );
            switch(sz)
            {
            case 8: std::swap( ptr[3], ptr[sz-4] );
                    std::swap( ptr[2], ptr[sz-3] );
            case 4: std::swap( ptr[1], ptr[sz-2] );
            case 2: std::swap( ptr[0], ptr[sz-1] );
            }
            return val;
        }
        //===============================================================================

        template <typename T> std::enable_if_t<std::is_arithmetic_v<T>, T>
        static to_little_endian( T val ) noexcept
        {
            return is_little() ? val : reverse_val( val );
        }

        template <typename T> std::enable_if_t<std::is_arithmetic_v<T>, T>
        static to_big_endian( T val ) noexcept
        {
            return is_big() ? val : reverse_val( val );
        }

        //===============================================================================
    } // endian
} // v
//=======================================================================================

#endif // VENDIAN_H
