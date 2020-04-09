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
#include "vtimer.h"

template<class> class TD;

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
void test_receiving_on_vcan0()
{
    vcan_socket s;
    try
    {
        s.bind( "vcan0" );
        s.send( 0x123, "12345678" );
    } catch (...)
    {
        vwarning << "Cannot bind to vcan0";
        vapplication::stop();
    }

    s.received += []( vcan_socket::message msg )
    {
        vdeb << msg;
    };

    //  пару секунд послушаем и остановим поллинг, начнем тест.
    vtimer timer;
    timer.start( std::chrono::seconds(2) );
    timer.timeout += vapplication::stop;

    vapplication::poll();
}
//=======================================================================================


//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    test_receiving_on_vcan0();

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
