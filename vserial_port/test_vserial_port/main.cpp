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

#include "vterminal_options.h"
#include "vserial_port.h"
#include "vapplication.h"
#include "vbyte_buffer.h"
#include "vlog.h"
//#include "vtimer.h"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class Test_vserial_port: public testing::Test
{};
#pragma GCC diagnostic pop

//=======================================================================================
//  К сожалению, тестировать без порта нереально, отлаживаюсь здесь.
TEST_F( Test_vserial_port, create_only_test )
{
    vserial_port port;
    (void) port;

    vterminal_options opts;
    (void) opts;

    // type here
}
//=======================================================================================


//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
//    vterminal_options t;
//    vserial_port port;
//    port.open("/dev/ttyUSB0");
//    port.set_options( vterminal_options{115200} );
//    port.received += [](std::string data)
//    {
//        vdeb << data;
//    };
//    port.err_hang_up += []
//    {
//        vdeb << "HANG UP";
//    };

//    vtimer timer; static int cnt = 0;
//    timer.timeout += [&]
//    {
//        vdeb << "timeout" << ++cnt;
//        //port.write("echo ololo");
//    };
//    timer.start( std::chrono::seconds(2) );

//    vapplication::poll();
//    return 0;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
