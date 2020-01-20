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
#include "vcat.h"
#include "vlog.h"

#include "impl_vposix/wrap_stdio.h"
#include "impl_vposix/wrap_fcntl.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/wrap_sys_file.h"
#include "impl_vposix/wrap_sys_epoll.h"
#include "impl_vposix/wrap_sys_eventfd.h"
#include "impl_vposix/wrap_arpa_inet.h"
#include "impl_vposix/wrap_termios.h"

#include "vapplication.h"
#include "vthread.h"
#include "vprocess.h"
#include "vthread_context.h"
#include <thread>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VPoll_Test: public testing::Test
{};
class VPosix_Test: public testing::Test
{};
#pragma GCC diagnostic pop


template<class> class TD;

//=======================================================================================

TEST_F( VPosix_Test, vprocess )
{

}

//=======================================================================================

TEST_F( VPosix_Test, 1 )
{
    using namespace impl_vposix;
    auto fd = wrap_fcntl::open_append( "test.txt" );
    wrap_unistd::write( fd, "1\n" );

    //  Хмм, можно переименовать открытый файл и продолжать в него писать.
    wrap_stdio::file_rename( "test.txt", "test.1.txt" );
    wrap_unistd::write( fd, "2\n" );

    //  Хмм, можно переписать тот открытый файл.
    auto fd2 = wrap_fcntl::open_append( "test.txt" );
    wrap_unistd::write( fd2, "3\n" );
    wrap_stdio::file_rename( "test.txt", "test.1.txt" );
}

//=======================================================================================

TEST_F( VPosix_Test, 2 )
{
    using namespace impl_vposix;

    // к сожалению, не умею делать блокировку файлов.
    auto fd1 = wrap_fcntl::open_append( "1.txt" );
    wrap_unistd::close( fd1 );

    //wrap_unistd::write( fd1, vcat(getpid(), "\n") );

    //wrap_sys_file::lock_exclusive( fd1 );
    //wrap_fcntl::set_write_lock( fd1 );
    //wrap_fcntl::get_print_lock( fd1 );
    //vdeb << "locked";
    //sleep(1000);
}

//=======================================================================================

TEST_F( VPosix_Test, 3 )
{
    using namespace std;
    using namespace impl_vposix;

    eventfd efd;
    epoll ep;
    int count = 0;
    struct R : epoll_receiver
    {
        eventfd *efd;
        int *count;
        void on_events( epoll_receiver::events ) override
        {
            while( efd->read() )
                ++(*count);
        }
    } r;
    r.efd = &efd;
    r.count = &count;

    ep.add( efd.handle(), epoll::In, &r );

    efd.write();
    efd.write();
    efd.write();
    efd.write();

    ep.wait_once();

    EXPECT_EQ( count, 4 );
}
//=======================================================================================

//=======================================================================================
TEST_F( VPoll_Test, vapplication )
{
    vapplication app;
    app.invoke( []{vdeb << 1;} );
    app.invoke( []{vdeb << 2;} );
    app.invoke( []{vdeb << 3;} );
    app.invoke( []{vdeb << 4;} );
    app.invoke( []{vdeb << 5;} );
    app.invoke( [](int i){vdeb << i;}, 42 );
    app.invoke( [&]{app.stop();} );

    app.poll();
}

//=======================================================================================
void f1()
{
    int a = -1, b = -1;
    vthread thread;
    thread.invoke( [&]
    {
        a = 1;
    });
    thread.invoke([&]
    {
        b = 2;
    });
    thread.join();
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 2);
}

TEST_F( VPoll_Test, 1 )
{
    for (int i = 0; i < 100; ++i)
    {
        f1();
    }
    vdeb << "before thread";
    vthread thread;
    vdeb << "after thread";

    thread.invoke( []
    {
        vdeb << "in l1";
        EXPECT_THROW( vthread::poll(), impl_vlog::error );
    });
    thread.invoke([]
    {
        vdeb << "in l2";
    });
}

//=======================================================================================
//  Были ошибки соревнования, которые детектировались подобным подходом.
static void jwe()
{
    vthread thread;
    thread.invoke( []{throw 42;} );

    int check = -1;
    try
    {
       // vdeb << "about join";
        thread.join();
    }
    catch (int i)
    {
        check = i;
    }
    EXPECT_EQ( check, 42 );
}

TEST_F( VPoll_Test, join_with_exception )
{
    auto one_ck = []
    {
        for (int i = 0; i < 10000; ++i)
            jwe();
    };
    one_ck();
    vthread t1( one_ck );
    vthread t2( one_ck );
}

//=======================================================================================
TEST_F( VPoll_Test, func_invokes )
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
TEST_F( VPoll_Test, class_invokes )
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
TEST_F( VPoll_Test, alternate_func )
{
    auto l = []
    {
        vdeb << "alt func";
        vthread::poll();
    };
    vthread thread(l);
}

//=======================================================================================
TEST_F( VPoll_Test, app_thread )
{
    vapplication app;

    int ck = -1;
    auto l = [&]
    {
        vdeb << "alt func";
        ck = 42;
        vapplication::stop();
    };
    vthread thread;
    thread.invoke( l );

    app.poll();
}

//=======================================================================================
TEST_F( VPoll_Test, wrap_termios )
{
    impl_vposix::wrap_termios tmio; // просто можем создать.
    (void) tmio;
}

//=======================================================================================
TEST_F( VPoll_Test, context )
{
    auto main_th_id = std::this_thread::get_id();

    vthread_context main_ctx;

    vthread th;
    th.invoke( [&]
    {
        EXPECT_NE( main_th_id, std::this_thread::get_id() );
        main_ctx.invoke( [&]
        {
            vdeb << "In main context...";
            EXPECT_EQ( main_th_id, std::this_thread::get_id() );
            vapplication::stop();
        }); // for main context
    }); // in thread

    vapplication::poll();
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
