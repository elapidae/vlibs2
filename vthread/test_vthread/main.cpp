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
#include "vlog.h"
#include "vthread.h"
#include "vpoll.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VThread_Test: public testing::Test
{};
#pragma GCC diagnostic pop

using namespace std;

//=======================================================================================

TEST_F( VThread_Test, 1 )
{
    vthread thread;
    thread.invoke( []
    {
        vdeb << "in l1";
        vpoll::poll();
    });
    thread.invoke([]
    {
        vdeb << "in l2";
    });
}

//=======================================================================================

TEST_F( VThread_Test, join_with_exception )
{
    vthread thread;
    thread.invoke( []{throw 42;} );

    int check = -1;
    try
    {
        vdeb << "about join";
        thread.join();
    }
    catch (int i)
    {
        check = i;
    }
    EXPECT_EQ( check, 42 );
}

//=======================================================================================

TEST_F( VThread_Test, func_invokes )
{
    vthread thread;

    int a0 = -1;
    int b1 = -1;
    int c1 = -1, c2 = -1;

    thread.invoke( [&]{ a0 = 1;} );
    thread.invoke( [&](int _1){b1 = _1;}, 2 );
    thread.invoke( [&](int _1, int _2){c1 = _1; c2 = _2;}, 3, 4 );

    thread.join();
    EXPECT_EQ( a0, 1 );
    EXPECT_EQ( b1, 2 );
    EXPECT_EQ( c1, 3 );
    EXPECT_EQ( c2, 4 );
}

//=======================================================================================

TEST_F( VThread_Test, class_invokes )
{
    vthread thread;

    struct S
    {
        int n = -1;
        void non() { n = 1; }

        int f = -1;
        void foo(int v) { f = v; }

        int b1 = -1, b2 = -1;
        void bar(int v1, int v2) { b1 = v1; b2 = v2; }
    } s;

    thread.invoke( &s, &S::non );
    thread.invoke( &s, &S::foo, 2 );
    thread.invoke( &s, &S::bar, 3, 4 );
    thread.join();
    EXPECT_EQ( s.n, 1 );
    EXPECT_EQ( s.f, 2 );
    EXPECT_EQ( s.b1, 3 );
    EXPECT_EQ( s.b2, 4 );
}

//=======================================================================================

TEST_F( VThread_Test, alternate_func )
{
    auto l = []
    {
        vdeb << "alt func";
        vpoll::poll();
    };
    vthread thread(l);
}

//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
//    vdeb << this_thread::get_id();
//    vthread thread;
//    thread.invoke( []{vdeb << this_thread::get_id() << 1;} );
//    thread.invoke( []{vdeb << this_thread::get_id() << 2;} );
//    thread.invoke( []{vdeb << this_thread::get_id() << 3;} );
//    return 0;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
