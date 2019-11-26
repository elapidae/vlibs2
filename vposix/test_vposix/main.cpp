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


#include <iostream>
#include <unordered_map>

#include "gtest/gtest.h"
#include "vcat.h"
#include "vlog.h"

#include "impl_vposix/wrap_stdio.h"
#include "impl_vposix/wrap_fcntl.h"
#include "impl_vposix/wrap_unistd.h"

class VPosix_Test: public testing::Test
{};

using namespace std;

template<class> class TD;


//=======================================================================================

TEST_F( VPosix_Test, _1_ )
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
#include <fcntl.h>
TEST_F( VPosix_Test, _2_ )
{
    using namespace impl_vposix;
    auto fd1 = wrap_fcntl::open_append( "t.txt" );

    //EXPECT_ANY_THROW( wrap_fcntl::open_write_append("t.txt") );

    auto flags = wrap_fcntl::file_control_get( fd1 );
    flags |= O_EXCL;
    wrap_fcntl::file_control_set( fd1, flags );
    vdeb << wrap_fcntl::file_control_get( fd1 );

    auto fd2 = wrap_fcntl::open_append( "t.txt" );

//    wrap_unistd::write( fd, "1\n" );
//    //  Хмм, можно переименовать открытый файл и продолжать в него писать.
//    wrap_stdio::file_rename( "test.txt", "test.1.txt" );
//    wrap_unistd::write( fd, "2\n" );
//    //  Хмм, можно переписать тот открытый файл.
//    wrap_unistd::write( fd2, "3\n" );
//    wrap_stdio::file_rename( "test.txt", "test.1.txt" );
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
