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


class VPosix_Test: public testing::Test
{};

template<class> class TD;

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
        void on_ready_read() override
        {
            while( efd->read() )
                ++(*count);
        }
    } r;
    r.efd = &efd;
    r.count = &count;

    ep.add_read( efd.handle(), &r );

    efd.write();
    efd.write();
    efd.write();
    efd.write();

    ep.wait_once();

    EXPECT_EQ( count, 4 );
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
