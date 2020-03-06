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
#include "vtcp_socket.h"
#include "vapplication.h"
#include "vtcp_server.h"
#include "vcat.h"
#include "vudp_socket.h"
#include "vbyte_buffer.h"

//=======================================================================================

TEST_F( VNetwork_Test, addresses )
{
    EXPECT_ANY_THROW( vsocket_address("bad") );

    EXPECT_EQ( vsocket_address::any_ip4().ip(), "0.0.0.0" );
    EXPECT_EQ( vsocket_address::any_ip6().ip(), "::" );

    EXPECT_EQ( vsocket_address::loopback_ip4().ip(), "127.0.0.1" );
    EXPECT_EQ( vsocket_address::loopback_ip6().ip(), "::1" );

    EXPECT_EQ( vsocket_address::any_ip4(1234).port(), 1234 );
    EXPECT_EQ( vsocket_address::any_ip6(1235).port(), 1235 );

    EXPECT_EQ( vsocket_address::loopback_ip4(1236).port(), 1236 );
    EXPECT_EQ( vsocket_address::loopback_ip6(1237).port(), 1237 );

    EXPECT_EQ( vsocket_address("127.128.129.130").ip(), "127.128.129.130" );
    EXPECT_EQ( vsocket_address("1234:5678::9abc:0d0").ip(), "1234:5678::9abc:d0" );
}

//=======================================================================================

TEST_F( VNetwork_Test, simple_tcp )
{
    std::string h, w;

    vtcp_socket::unique_ptr ss;
    vtcp_server serv;
    serv.accepted += [&](vtcp_socket::accepted_peer peer)
    {
        ss = peer.as_unique();
        ss->received += [&](std::string msg)
        {
            vdeb << ss->address();
            vdeb << ss->peer_address();
            h = msg;
            ss->send( "world" );
        };
    };
    serv.listen_loopback_ip6();

    vtcp_socket sock;

    // will bad connection
    //sock.connect( vsocket_address::loopback_ip6(12345) );
    sock.connect( vsocket_address::loopback_ip6(serv.address().port()) );

    sock.connected += [&]
    {
        sock.send( "hello" );
    };
    sock.received += [&](std::string msg)
    {
        w = msg;
        vapplication::stop();
    };
    sock.disconnected += [&]
    {
        vfatal << "Bad connection.";
        vapplication::stop();
    };

    vapplication::poll();

    EXPECT_EQ( h, "hello" );
    EXPECT_EQ( w, "world" );
}

//=======================================================================================

void sock_server()
{
    vtcp_server s;
    s.listen_any_ip4( 1234 );

    vtcp_socket::unique_ptr serv_sock;
    int cnt = 0;
    s.accepted += [&]( vtcp_socket::accepted_peer peer )
    {
        serv_sock = peer.as_unique();
        serv_sock->disconnected     += []{ vdeb << "serv disconn"; };
        serv_sock->err_broken_pipe  +=
        []{
            vdeb << "serv br pipe";
            vapplication::stop();
        };
        serv_sock->err_conn_refused += []{ vdeb << "serv refused"; };
        serv_sock->received += [&]( std::string data )
        {
            vdeb << "server received" << data;
            serv_sock->send( vcat("world",cnt++) );
        };
    };

    vapplication::poll();
}

//=======================================================================================
TEST_F( VNetwork_Test, simple_udp )
{
    std::string s1_msg;

    vudp_socket s1, s2;
    s1.bind_loopback_ip6();

    s1.received += [&]( vudp_socket::packet pkt )
    {
        s1_msg = pkt.data;
        vdeb << pkt.peer;
        vapplication::stop();
    };

    s2.send_to( s1.address(), "Hello world!" );

    vapplication::poll();
    EXPECT_EQ( s1_msg, "Hello world!" );
}

//=======================================================================================
TEST_F( VNetwork_Test, raw_conversions )
{
    auto addr = vsocket_address::from_raw_ip4( 0xc0a80019, 123 );
    EXPECT_EQ( addr.ip(), "192.168.0.25" );
    EXPECT_EQ( addr.port(), 123 );

    vbyte_buffer b;
    b.append('\0');     // first octave == 0010
    b.append('\20');    // 020 == 0x10
    for (int i = 2; i < 15; ++i ) b.append('\0');
    b.append('\21');    // 021 == 0x11
    EXPECT_EQ( b.size(), 16 );

    addr = vsocket_address::from_raw_ip6( b.data(), 234 );
    EXPECT_EQ( addr.ip(), "10::11" );
    EXPECT_EQ( addr.port(), 234 );
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
