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

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VPoll_Test: public testing::Test
{};
#pragma GCC diagnostic pop

#include <future>
#include "vapplication.h"

#include "vpoll.h"

//=======================================================================================

//TEST_F( VPoll_Test, 1 )
void fake()
{
    std::future<void> f1 = std::async(std::launch::async, []
    {
        vdeb << this_thread::get_id();
        vpoll::poll();
    });
    std::future<void> f2 = std::async(std::launch::async, []
    {
        vdeb << this_thread::get_id();
        vpoll::poll();
        throw 42;
    });

    try {
        f2.get();
    } catch (int i) {
        vdeb << i;
    }
    f1.get();
}

//=======================================================================================

TEST_F( VPoll_Test, _test_app )
{
    vapplication.invoke( []{ vdeb << "Hello"; } );
    vapplication.invoke( []{ vdeb << "World"; } );
    vapplication.invoke( []{ vapplication.stop(); } );

    vpoll::poll();
    vdeb << "stopped";
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
