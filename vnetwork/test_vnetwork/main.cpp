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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VNetwork_Test: public testing::Test
{};
#pragma GCC diagnostic pop

#include "vlog.h"

//=======================================================================================
#include "vinet_address.h"

TEST_F( VNetwork_Test, inet_addr )
{
    vinet_address a4( "1.2.3.4" );
    EXPECT_EQ( a4.str(), "1.2.3.4" );

    vinet_address a6( "1:2:3::4:5:6" );
    EXPECT_EQ( a6.str(), "1:2:3::4:5:6" );

    EXPECT_ANY_THROW( vinet_address("bad") );

    auto bad = vinet_address::from_string( "bad 2" );
    EXPECT_TRUE( !bad.is_valid() );
}
//=======================================================================================

//=======================================================================================
#include <sys/socket.h>
#include <arpa/inet.h>

TEST_F( VNetwork_Test, sock )
{
    vdeb << sizeof(sockaddr);
    vdeb << sizeof(sockaddr_in);
    vdeb << sizeof(sockaddr_in6);
    vdeb << sizeof(sockaddr_storage);
}
//=======================================================================================


#include "vtcp_socket.h"
#include "vapplication.h"
#include <QTcpServer>

//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    QTcpServer serv;
    auto ok = serv.listen( QHostAddress::Any, 1234 );
    Q_ASSERT( ok );

    vtcp_socket s;
    //s.connect( vsocket_address::any_ip4(1234) );
    s.connect( vsocket_address::loopback_ip4(1234) );

    vapplication::poll();
    return 1;

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
