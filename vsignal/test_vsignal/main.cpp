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

#include "gtest/gtest.h"
#include <iostream>

#include "vsignal.h"


using namespace std;

//=======================================================================================
#define vdeb std::cout << basename(__FILE__) << ":" << __LINE__ << "==> "
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VSignal_Test: public testing::Test
{};
#pragma GCC diagnostic pop
//=======================================================================================


//=======================================================================================

TEST_F( VSignal_Test, simple1 )
{
    vsignal<> s;

    bool ok = false;
    s += [&]
    {
        ok = true;
    };

    s();
    EXPECT_TRUE( ok );
}

//=======================================================================================

TEST_F( VSignal_Test, simple2 )
{
    vsignal<> s;

    int count = 0;
    auto lid1 = s += [&] { ++count; };
    auto lid2 = s.link( [&]{++count;} );
    s();
    EXPECT_EQ( count, 2 );

    s -= lid1;
    s();
    EXPECT_EQ( count, 3 );

    s.unlink( lid2 );
    s();
    EXPECT_EQ( count, 3 );
}

//=======================================================================================

TEST_F( VSignal_Test, simple3 )
{
    vsignal<int> s;

    int count = 42;

    vsignal<int>::link_id id;
    for (int i = 0; i < 1000; ++i)
    {
        s += [&](int){--count;};
        id = s.link( [&](int){++count;} );
    }
    s(1);
    EXPECT_EQ( count, 42 );
}

//=======================================================================================

//  Проверка, что старые компиляторы могут
TEST_F( VSignal_Test, simple_link_class )
{
    vsignal<int,double,char> sig;

    struct Foo
    {
        void bar(int i, float f, char c)
        {
            vdeb << i << " " << f << " " << c << endl;
        }
    };
    Foo foo;

    sig.link( &foo, &Foo::bar );
    sig( 42, 3.14, 'A' );
}

//=======================================================================================

TEST_F( VSignal_Test, simple4 )
{
    vsignal<std::string> s;

    string s1 = "yes";
    string s2 = "no";
    s += [&s1](string ss){ s1 = ss; };
    s += [&s2](string ss){ s2 = ss; };

    // must work with move. VSignal shouldn't kill variable.
    s( std::move(string("ololo")) );

    EXPECT_EQ( s1, s2 );
    EXPECT_EQ( s1, "ololo" );
}

//=======================================================================================

TEST_F( VSignal_Test, check_link_wrong )
{
    vsignal<std::string> s1;
    vsignal<std::string> s2;

    auto l1 = s1.link( [](string){} );
    auto l2 = s2.link( [](string){} );

    EXPECT_THROW( s1.unlink(l2), std::runtime_error );
    EXPECT_THROW( s2.unlink(l1), std::runtime_error );

    s1.unlink(l1);
    s2.unlink(l2);
}

//=======================================================================================

TEST_F( VSignal_Test, classes )
{
    struct Cls
    {
        int foo = 0;
        void bar() { ++foo; }
    } cls;

    vsignal<> s;

    s.link( &cls, &Cls::bar );
    s();

    EXPECT_EQ( cls.foo, 1 );
}

//=======================================================================================

TEST_F( VSignal_Test, unlink )
{
    vsignal<> s;

    auto lid = s.link( []{} );
    s.unlink( lid );

    EXPECT_THROW( s.unlink(lid), std::runtime_error );
    EXPECT_THROW( s -= lid, std::runtime_error );
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
