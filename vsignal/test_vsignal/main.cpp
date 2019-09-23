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
class VSignal_Test: public testing::Test
{};
#pragma GCC diagnostic pop
//=======================================================================================


//=======================================================================================

TEST_F( VSignal_Test, simple1 )
{
    VSignal<> s;

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
    VSignal<> s;

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
    VSignal<int> s;

    int count = 42;

    VSignal<int>::link_id id;
    for (int i = 0; i < 1000; ++i)
    {
        s += [&](int){--count;};
        id = s.link( [&](int){++count;} );
    }
    s(1);
    EXPECT_EQ( count, 42 );
}

//=======================================================================================

TEST_F( VSignal_Test, simple4 )
{
    VSignal<std::string> s;

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

TEST_F( VSignal_Test, classes )
{
    struct Cls
    {
        int foo = 0;
        void bar() { ++foo; }
    } cls;

    VSignal<> s;

    s.link( &cls, &Cls::bar );
    s();

    EXPECT_EQ( cls.foo, 1 );
}

//=======================================================================================

TEST_F( VSignal_Test, unlink )
{
    VSignal<> s;

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
