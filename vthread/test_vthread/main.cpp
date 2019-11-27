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


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VThread_Test: public testing::Test
{};
#pragma GCC diagnostic pop

#include <thread>

using namespace std;

//=======================================================================================
TEST_F( VThread_Test, join_with_exception )
{
    vthread thread;
    thread.invoke( []{throw 42;} );

    int check = -1;
    try
    {
        thread.join();
    } catch (int i)
    {
        check = i;
    }
    EXPECT_EQ( check, 42 );
}
//=======================================================================================


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
