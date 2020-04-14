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
TEST_F( TEST_VSETTINGS, vsettings_simple_use )
{
    vsettings settings;

    auto val1 = "val1";
    auto val2 = 42;
    auto val3 = 3.1415926;

    settings.set( "key1", val1 );
    settings.set( "key2", val2 );
    settings.set( "key3", val3 );

    auto sub_group = settings.subgroup( "SUBGROUP" );
    sub_group.set( "key1", val1 );
    sub_group.set( "key2", val2 );
    sub_group.set( "key3", val3 );

    //vdeb << settings.to_ini();

    EXPECT_EQ( settings.get("key1"), val1 );
    EXPECT_EQ( settings.get<decltype(val2)>("key2"), val2 );
    EXPECT_DOUBLE_EQ( settings.get<decltype(val3)>("key3"), val3 );

    EXPECT_EQ( settings.subgroup("SUBGROUP").get("key1"), val1 );
    EXPECT_EQ( settings.subgroup("SUBGROUP").get<decltype(val2)>("key2"), val2 );
    EXPECT_DOUBLE_EQ( settings.subgroup("SUBGROUP").get<decltype(val3)>("key3"), val3 );
}
//=======================================================================================
TEST_F( TEST_VSETTINGS, vsettings_schema_simple_use )
{
    vsettings::schema sh;

    string val1 = "val1";
    auto val2 = 42;
    auto val3 = 3.1415926;

    sh.add( "key1", &val1 );
    sh.add( "key2", &val2 );
    sh.add( "key3", &val3 );

    //  Формируем настройки из текущих значений.
    auto sett = sh.build();

    val1.clear();
    val2 = -1;
    val3 = -1;

    //  Устанавливаем значения согласно настройкам.
    sh.capture( sett );

    EXPECT_EQ( val1, "val1" );
    EXPECT_EQ( val2, 42 );
    EXPECT_DOUBLE_EQ( val3, 3.1415926 );

    //  Все поля обязательны, чтение должно "навернуться".
    EXPECT_TRUE( sett.del_key("key3") );
    EXPECT_ANY_THROW( sh.capture(sett) );
}
//=======================================================================================
//  Тест на то, чтобы пользователь случайно не добавил один и тот же ключ в группу.
TEST_F( TEST_VSETTINGS, same_key )
{
    vsettings::schema s;

    int i1, i2, i3;

    s.add( "1", &i1 );

    s.subgroup( "G" );
    s.add( "1", &i2 ); // It is OK.

    s.end_subgroup();
    EXPECT_ANY_THROW( s.add("1", &i3) );
}
//=======================================================================================
//  Тест на то, чтобы пользователь случайно не добавил один и тот же указатель в схему.
TEST_F( TEST_VSETTINGS, same_ptr )
{
    vsettings::schema s;

    int i1;

    s.add( "1", &i1 );

    s.subgroup( "G" );
    EXPECT_ANY_THROW( s.add("2", &i1) );

    s.end_subgroup();
    EXPECT_ANY_THROW( s.add("3", &i1) );
}
//=======================================================================================
TEST_F( TEST_VSETTINGS, escape_vals )
{
    vsettings s;

    string val;
    for ( int i = numeric_limits<char>::min(); i <= numeric_limits<char>::max(); ++i )
        val.push_back( i );

    s.set( "1", val );

    auto ini = s.to_ini();
    //vdeb << ini;

    vsettings r;
    r.from_ini( ini );
    auto ck = r.get( "1" );

    EXPECT_EQ( val, ck );
}
//=======================================================================================
//  2020-04-14 обнаружился баг, что при чтении строки из схемы, читается не вся строка,
//  а только часть до пробела, потому-что работает не через метод str get(), а через
//  метод get<string>().
TEST_F( TEST_VSETTINGS, bug_reading_string )
{
    const std::string val = "bug fix \t fix!";
    std::string v = val;

    vsettings::schema sh;
    sh.add("key", &v);

    auto ini = sh.build().to_ini();

    v.clear();
    vsettings sett;
    sett.from_ini( ini );
    sh.capture( sett );

    EXPECT_EQ( v, val );

    sett.set( "2", val );
    EXPECT_EQ( sett.get("2"), val );
    EXPECT_EQ( sett.get<std::string>("2"), val );
}
//=======================================================================================
TEST_F( TEST_VSETTINGS, test_reading_bool )
{
    bool t = true;
    bool f = false;

    vsettings::schema sh;
    sh.add("true", &t);
    sh.add("false", &f);

    auto ini = sh.build().to_ini();

    vsettings sett;
    sett.from_ini( ini );

    std::swap( t, f );
    sh.capture( sett );

    EXPECT_EQ( t, true );
    EXPECT_EQ( f, false );

    sett.set("on", "on");
    sett.set("off", "off");

    EXPECT_EQ( sett.get<bool>("on"),    true  );
    EXPECT_EQ( sett.get<bool>("off"),   false );
    EXPECT_EQ( sett.get<bool>("true"),  true  );
    EXPECT_EQ( sett.get<bool>("false"), false );
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
    bool t = true;
    bool f = false;

    vsettings::schema sh;
    sh.add("true", &t);
    sh.add("false", &f);

    auto ini = sh.build().to_ini();

    vsettings sett;
    sett.from_ini( ini );

    std::swap( t, f );
    sh.capture( sett );

    sett.set("on", "on");
    sett.set("off", "off");

    EXPECT_EQ( sett.get<bool>("on"),  true  );
    EXPECT_EQ( sett.get<bool>("off"), false );

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
//=======================================================================================
//  Main, do not delete...
//=======================================================================================
