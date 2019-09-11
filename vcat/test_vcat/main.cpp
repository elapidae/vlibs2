/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  Authors:
**  Alexandre Gromtsev aka elapidae     elapidae@yandex.ru
**  Nadezhda Churikova aka claorisel    claorisel@gmail.com
**  Ekaterina Boltenkova aka kataretta  kitkat52@yandex.ru
**  Ivan Deylid aka sid1057             ivanov.dale@gmail.com>
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#include "gtest/gtest.h"
#include <iostream>
#include "vcat.h"
#include <vector>
#include <list>
#include <map>


using namespace std;


class VCat_Test: public testing::Test
{};

//=======================================================================================

TEST_F( VCat_Test, hello_world )
{
    EXPECT_EQ( vcat().str(), "" );
    EXPECT_EQ( vcat("Hello world!").str(), "Hello world!" );
    EXPECT_EQ( vcat("Привет мир!").str(), "Привет мир!" );

    EXPECT_EQ( vcat(42).str(), "42" );
    EXPECT_EQ( vcat(42.3).str(), "42.3" );
    EXPECT_EQ( vcat(42.34f).str(), "42.34" );
    EXPECT_EQ( vcat(-42ll).str(), "-42" );
    EXPECT_EQ( vcat('a').str(), "a" );

    string s1 = "ololo";
    EXPECT_EQ( vcat(s1).str(), "ololo" );
    EXPECT_EQ( vcat(s1).str(), "ololo" );
    EXPECT_EQ( vcat(s1).str(), "ololo" );
}

//=======================================================================================

TEST_F( VCat_Test, hello_modifiers )
{
    EXPECT_EQ( (vcat().oct() << 42).str(), "052" );
    EXPECT_EQ( (vcat().dec() << 42).str(), "42" );
    EXPECT_EQ( (vcat().hex() << 42).str(), "0x2a" );

    EXPECT_EQ( vcat(true).str(), "true" );
    EXPECT_EQ( vcat(false).str(), "false" );

    EXPECT_EQ( vcat().num(42, 7, '=').str(), "=====42" );
}

//=======================================================================================

TEST_F( VCat_Test, hello_delimiters )
{
    //  В начале пробела появится не должно.
    EXPECT_EQ( vcat().space()(42).str(), "42" );

    EXPECT_EQ( vcat().space()(42,43).str(), "42 43" );
    EXPECT_EQ( vcat().space().nospace()(42,43).str(), "4243" );

    EXPECT_EQ( vcat().delimiter(',')(42,43,44).str(), "42,43,44" );
    EXPECT_EQ( vcat().delimiter(',').nospace()(42,43).str(), "4243" );

    EXPECT_EQ( vcat().space()(42,43).nospace()(44,45).str(), "42 434445" );

    EXPECT_EQ( vcat().space()("Abra","cadabra").str(), "Abra cadabra" );
    EXPECT_EQ( vcat().delimiter('+')("Abra","cadabra").str(), "Abra+cadabra" );
}
//=======================================================================================

TEST_F( VCat_Test, hello_tuple )
{
    EXPECT_EQ( vcat(pair<int,uint>()).str(), "{0,0}" );

    EXPECT_EQ( vcat(tuple<>()).str(), "{}" );
    EXPECT_EQ( vcat(tuple<int>(-1)).str(), "{-1}" );
    EXPECT_EQ( vcat(tuple<int,uint>()).str(), "{0,0}" );
    EXPECT_EQ( vcat(tuple<int,uint,int16_t>()).str(), "{0,0,0}" );

    EXPECT_EQ( vcat(tuple<string>("ololo")).str(), "{ololo}" );
}

//=======================================================================================

TEST_F( VCat_Test, hello_containers )
{
    EXPECT_EQ( vcat()(vector<bool>()).str(), "[]" );
    EXPECT_EQ( vcat()(vector<long>()).str(), "[]" );
    EXPECT_EQ( vcat()(vector<double>()).str(), "[]" );

    vector<int> vi{42, 43, 44, -45};
    EXPECT_EQ( vcat()(vi).str(), "[42,43,44,-45]" );
    EXPECT_NE( vcat()(vi).str(), "[42,43,44,-45,]" );

    vector<bool> vb{true,false,true};
    EXPECT_EQ( vcat()(vb).str(), "[true,false,true]" );

    list<int> li{42, 43, 44, -45};
    EXPECT_EQ( vcat()(li).str(), "[42,43,44,-45]" );

    map<int,double> mid;
    EXPECT_EQ( vcat()(mid).str(), "[]" );
    mid.insert( {42, 42.5} );
    EXPECT_EQ( vcat()(mid).str(), "[{42,42.5}]" );
    mid.insert( {43, 43.5} );
    EXPECT_EQ( vcat()(mid).str(), "[{42,42.5},{43,43.5}]" );
}

//=======================================================================================

TEST_F( VCat_Test, simple_from_text )
{
    EXPECT_EQ( vcat::from_text<long>("123456"), 123456 );
    EXPECT_EQ( vcat::from_text<string>("ololo"), "ololo" );

    EXPECT_FLOAT_EQ( vcat::from_text<float>("     -12.34"), -12.34 );

    EXPECT_DOUBLE_EQ( vcat::from_text<double>("-12.3456      "), -12.3456 );

    EXPECT_THROW( vcat::from_text<int>("ololo"), runtime_error );
}

//=======================================================================================



//=======================================================================================
//=======================================================================================
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//=======================================================================================
