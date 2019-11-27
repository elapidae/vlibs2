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
class VThread_Test: public testing::Test
{};
#pragma GCC diagnostic pop

#include <future>

#include "impl_vpoll/real_poll.h"

//=======================================================================================
TEST_F( VThread_Test, _test_name )
{
    std::future<void> f1 = std::async(std::launch::async, []
    {
        vdeb << this_thread::get_id();
        impl_vpoll::real_poll::poll();
    });
    std::future<void> f2 = std::async(std::launch::async, []
    {
        vdeb << this_thread::get_id();
        impl_vpoll::real_poll::poll();
        throw 42;
    });

    try {
        f2.get();
    } catch (int i) {
        vdeb << i;
    }


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
