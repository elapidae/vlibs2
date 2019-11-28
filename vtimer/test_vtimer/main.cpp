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
#include "vtimer.h"
#include "vpoll.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VTimer_Test: public testing::Test
{};
#pragma GCC diagnostic pop

using namespace std::chrono;

//=======================================================================================

TEST_F( VTimer_Test, 1 )
{
    vtimer t;
    t.singleshot( nanoseconds(1) );

    int check = -1;
    t.timeout += [&]
    {
        check = 42;
        vpoll::stop();
    };
    t.timeout_times += [](uint64_t times)
    {
        vdeb << times;
    };

    vpoll::poll();

    EXPECT_EQ( check, 42 );
}

//=======================================================================================

TEST_F( VTimer_Test, 2 )
{
    vtimer t( nanoseconds(1) );

    int count = -1;
    t.timeout_times += [&](uint64_t times)
    {
        vdeb << "times:" << times;
        if (++count == 5)
            vpoll::stop();
    };

    vpoll::poll();

    EXPECT_EQ( count, 5 );
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
