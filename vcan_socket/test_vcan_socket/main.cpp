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

#include "vcan_socket.h"
#include "vbyte_buffer.h"
#include "vapplication.h"
#include "vlog.h"

template<class> class TD;

//using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class vcan_socket_Test: public testing::Test
{};
#pragma GCC diagnostic pop


//=======================================================================================
TEST_F( vcan_socket_Test, ctors )
{
    vcan_socket can;
}
//=======================================================================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

//#include "impl_vposix/wrap_net_if.h"

//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    vcan_socket s;
    s.bind( "vcan0" );
    s.send( 0x123, "12345678" );

    s.received += []( vcan_socket::message msg )
    {
        vdeb << msg;
//        vdeb.aligned(msg.id, 5)
//            .aligned(vbyte_buffer(msg.data).to_Hex(), 3*8 + 5)
//            .aligned(msg.iface, 7)
//            .aligned(msg.rx, 5)
//            .aligned(msg.tx, 5);
    };

    vapplication::poll();

//    auto s = socket(PF_CAN, SOCK_RAW, CAN_RAW);

//    for (auto& s: impl_vposix::wrap_net_if::interfaces())
//        vdeb << s.index << s.name;

//    return 0;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
