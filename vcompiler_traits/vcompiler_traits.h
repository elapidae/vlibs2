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

#ifndef VCOMPILER_TRAITS_H
#define VCOMPILER_TRAITS_H

#include "details/vendian.h"
#include "details/vpretty_function.h"

//=======================================================================================
//  Пока все выводится от компилятора, надо найти список стандартных предопределенных
//  макросов.
#ifdef __GNUC__
    //===================================================================================
    #define V_GNUC_COMPILER_VERSION ( (__GNUC__            * 0x10000) + \
                                      (__GNUC_MINOR__      *   0x100) + \
                                      (__GNUC_PATCHLEVEL__          )   \
                                    )
    //===================================================================================
#else
    //===================================================================================
    //#error "Unknown compiler"
    //===================================================================================
#endif // __GNUC__
//=======================================================================================
#if defined __MINGW32__ || __MINGW64__
    #define V_MINGW 1
#else
    #define V_MINGW 0
#endif

//=======================================================================================
// V_NOEXCEPT_SIGNATURE_WARNING_ENABLED -- выставляется когда версия компилятора
//  достаточно высока, чтобы выдать такое сообщение: "<надо еще получить :(>"
//  NB! С версией не уверен.
#define V_NOEXCEPT_SIGNATURE_WARNING_ENABLED (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
// V_NORETURN_ENABLED -- выставляется когда компилятор в состоянии опознать [[noreturn]].
//  NB! С версией не уверен.
//  Use V_NORETURN macro instead of real [[noreturn]] :((
#define V_NORETURN_ENABLED (V_GNUC_COMPILER_VERSION > 0x040703)
#if V_NORETURN_ENABLED
    #define V_NORETURN [[noreturn]]
#else
    #ifndef _MSC_VER
        #define V_NORETURN __attribute__((noreturn))
    #else
        #define V_NORETURN //MSVC howto?
    #endif
#endif
//=======================================================================================


//=======================================================================================
// V_CONTAINERS_HAS_EMPLACE -- выставляется когда контейнеры стандартной библиотеки,
//  такие как map, set, unordered_map etc обучены emplace().
//  NB! С версией не уверен.
#define V_CONTAINERS_HAS_EMPLACE (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
// V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS -- может работать с variadic templates с лямбдами.
//  См. vpoll/vinvoke_iface
//  NB! С версией не уверен.
#define V_CAN_VARIADIC_TEMPLATES_IN_LAMBDAS (V_GNUC_COMPILER_VERSION > 0x040804)
//=======================================================================================


//=======================================================================================
//  Старые компиляторы не умеют конструкторы-прокси.
//  NB! С версией не уверен.
#define V_CAN_PROXY_CONSTRUCTORS (V_GNUC_COMPILER_VERSION > 0x040804)
//=======================================================================================


//=======================================================================================
// V_COMPILER_KNOWS_THREAD_LOCAL -- знает keyword thread_local.
//  См. vapplication/vpoll
//  NB! С версией не уверен.
#define V_COMPILER_KNOWS_THREAD_LOCAL (V_GNUC_COMPILER_VERSION > 0x040703)
//=======================================================================================


//=======================================================================================
//  V_HAS_GET_TIME -- флаг-костыль, что компилятор знает std::get_time().
//  Deprecated, в vchrono стал использовать только posix функции.
//  UPD 2020-11-04 надо вернуть get_time, т.к. POSIX винде не указ, а туда надо.
//  NB! Нет никаких внятных способов понять есть ли std::get_time(), никто об
//  этом не сообщает, никто ничего не гарантирует.
//  Действуем так: проверяем на нужных нам компиляторах, остальные -- фтопку.
#ifndef _MSC_VER
    #define V_HAS_STD_GET_TIME ((V_MINGW) || (V_GNUC_COMPILER_VERSION > 0x040703))
#else
    #define V_HAS_STD_GET_TIME 1
#endif
//=======================================================================================


//=======================================================================================
//  Для подавления предупреждений от компилятора.
//  Новые версии выводят предупреждение, а старые компиляторы прагму не знают.
//  NB! С версией не уверен.
#if  (V_GNUC_COMPILER_VERSION >= 0x070000)
    #define V_PRAGMA_GCC_KNOWS_IMPLICIT_FALLTHROUGH
#endif
//=======================================================================================


//=======================================================================================
//  Для подавления предупреждений от компилятора.
//  Новые версии выводят предупреждение, а старые компиляторы прагму не знают.
//  NB! С версией не уверен.
#if  (V_GNUC_COMPILER_VERSION >= 0x070000)
    #define V_PRAGMA_GCC_KNOWS_NOEXCEPT_TYPE
#endif
//=======================================================================================





#endif // VCOMPILER_TRAITS_H
