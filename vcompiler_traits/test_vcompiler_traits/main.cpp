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

// TODO!
//  Использован linux-овский popen/pclose для чтения подпроцессов. Не особо грамотно,
//  хотелось бы разобраться, чтобы были грамотные обертки для работы с процессами.

#include "gtest/gtest.h"
#include <iostream>

#include "vcompiler_traits.h"
#include "vvoid_type.h"

using namespace std;

//=======================================================================================
std::string last_fname(const char *filepath)
{
    std::string fp(filepath);
    auto pos = fp.find_last_of( '/' );
    if ( pos == std::string::npos ) return fp;
    return fp.substr( pos + 1 );
}
#define vdeb std::cout << last_fname(__FILE__) << ":" << __LINE__ << "==> "
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VCompiler_Traits_Test: public testing::Test
{};
#pragma GCC diagnostic pop
//=======================================================================================


//=======================================================================================

TEST_F( VCompiler_Traits_Test, gnuc_only )
{
    EXPECT_EQ(  V_GNUC_COMPILER_VERSION        &0xFF, __GNUC_PATCHLEVEL__   );
    EXPECT_EQ( (V_GNUC_COMPILER_VERSION >> 8)  &0xFF, __GNUC_MINOR__        );
    EXPECT_EQ( (V_GNUC_COMPILER_VERSION >> 16) &0xFF, __GNUC__              );
}

//=======================================================================================
template<typename T, typename = void>
struct test_vvoid_type : public std::false_type
{};

template<typename T>
struct test_vvoid_type<T, vvoid_type<int> > : public std::true_type
{};
//---------------------------------------------------------------------------------------
struct SFoo { static void foo(); };
//---------------------------------------------------------------------------------------
template<typename T, typename = void>
struct has_foo : public std::false_type {};
template<typename T>
struct has_foo<T, vvoid_type<decltype(T::foo)> > : public std::true_type {};
//---------------------------------------------------------------------------------------
template<typename T, typename = void>
struct has_bar : public std::false_type {};

template<typename T>
struct has_bar<T, vvoid_type<decltype(T::bar)> > : public std::true_type {};
//---------------------------------------------------------------------------------------
TEST_F( VCompiler_Traits_Test, vvoid_type )
{
    EXPECT_TRUE( test_vvoid_type<uint>::value );
    EXPECT_TRUE( has_foo<SFoo>::value );
    EXPECT_FALSE( has_bar<SFoo>::value );
}

#ifdef V_DO_NOT_KNOW_STD_VOID_T
#else
//ddd
#endif
//=======================================================================================




//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
