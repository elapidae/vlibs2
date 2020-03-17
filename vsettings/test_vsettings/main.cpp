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
#include "vsettings.h"
#include "vlog.h"

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class TEST_VSETTINGS: public testing::Test
{};
#pragma GCC diagnostic pop

//=======================================================================================
TEST_F( TEST_VSETTINGS, 1 )
{
}
//=======================================================================================
//  EXPECT_TRUE
//
//  EXPECT_EQ
//  EXPECT_FLOAT_EQ
//  EXPECT_DOUBLE_EQ
//
//  EXPECT_THROW
//  EXPECT_ANY_THROW
//  EXPECT_NO_THROW


//=======================================================================================
//  Main, do not delete...
//=======================================================================================
int main(int argc, char *argv[])
{
    vsettings s;
    s.set( "123", "456" );
    s.set( "hello", "world" );

    {
    auto sg = s.subgroup("SUB");
    sg.set( "321", "456" );
    sg.set( "098", "world" );
    auto sg2 = sg.subgroup("SSG");
    sg2.set( "sskey", "ssval" );
    }
    {
    auto sg = s.subgroup("SUB-2");
    sg.set( "321", "456" );
    sg.set( "098", "world" );
    }

    vdeb << s.save();

    return 0;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
