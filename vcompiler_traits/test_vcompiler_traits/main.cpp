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
#include "details/vendian.h"

using namespace std;

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
//  In old compilers might not work std::void_t, let's test std::void_t.
template<typename T, typename = void>
struct test_void_t : public std::false_type
{};

template<typename T>
struct test_void_t<T, std::void_t<int> > : public std::true_type
{};
//---------------------------------------------------------------------------------------
struct SFoo { static void foo(); };
//---------------------------------------------------------------------------------------
template<typename T, typename = void>
struct has_foo : public std::false_type {};
template<typename T>
struct has_foo<T, std::void_t<decltype(T::foo)> > : public std::true_type {};
//---------------------------------------------------------------------------------------
template<typename T, typename = void>
struct has_bar : public std::false_type {};

template<typename T>
struct has_bar<T, std::void_t<decltype(T::bar)> > : public std::true_type {};
//---------------------------------------------------------------------------------------
TEST_F( VCompiler_Traits_Test, vvoid_type )
{
    EXPECT_TRUE( test_void_t<uint>::value );
    EXPECT_TRUE( has_foo<SFoo>::value );
    EXPECT_FALSE( has_bar<SFoo>::value );
}

//=======================================================================================

union _test_endian
{
    uint32_t u32 = 0x12345678;
    struct
    {
        uint8_t b1,b2,b3,b4;
    } bytes;
};
static constexpr bool little_endian()
{
    constexpr _test_endian te{ 0x12345678 };
    return
        te.bytes.b1 == 0x78 &&
        te.bytes.b2 == 0x56 &&
        te.bytes.b3 == 0x34 &&
        te.bytes.b4 == 0x12;
}
static constexpr bool big_endian()
{
    constexpr _test_endian te{ 0x12345678 };
    return
        te.bytes.b1 == 0x12 &&
        te.bytes.b2 == 0x34 &&
        te.bytes.b3 == 0x56 &&
        te.bytes.b4 == 0x78;
}

TEST_F( VCompiler_Traits_Test, endian )
{
    auto l = little_endian();
    auto b = big_endian();
    auto lonly = l && !b;
    auto bonly = b && !l;

    auto _or  = lonly || bonly;
    auto _and = lonly && bonly;

    EXPECT_TRUE ( _or  );
    EXPECT_FALSE( _and );

    if ( l ) cout << "little endian" << endl;
    if ( b ) cout << "BIG endian"    << endl;

    auto ll = v::endian::is_little();
    auto bb = v::endian::is_big();

    EXPECT_TRUE ( ll == l );
    EXPECT_TRUE ( bb == b );
}
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
