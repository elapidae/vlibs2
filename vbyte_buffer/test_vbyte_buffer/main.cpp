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

#include "vbyte_buffer.h"
#include "vbyte_buffer_view.h"

template<class> class TD;

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VByteBuffer_Test: public testing::Test
{};
#pragma GCC diagnostic pop


//=======================================================================================
TEST_F( VByteBuffer_Test, ctors )
{
    vbyte_buffer bb_empty;
    EXPECT_EQ( bb_empty.str(), string{} );

    vbyte_buffer bb( "ololo" );
    EXPECT_EQ( bb.str(), string{"ololo"} );

    // for operator string().
    string s = bb;
    EXPECT_TRUE( s == string("ololo") );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, compare )
{
    vbyte_buffer b1{"123"};
    vbyte_buffer b2{"123"};
    vbyte_buffer b3{"12"};
    EXPECT_EQ( b1, b1 );
    EXPECT_EQ( b1, b2 );
    EXPECT_NE( b1, b3 );
    EXPECT_NE( b2, b3 );

    EXPECT_TRUE ( b1 == b2 );
    EXPECT_FALSE( b1 == b3 );

    EXPECT_TRUE ( b1 != b3 );
    EXPECT_FALSE( b1 != b2 );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, appends )
{
    vbyte_buffer bb;
    uint8_t u8 = 'U';
    int8_t  i8 = 'I';
    char c = 'C';

    bb.append( u8 );
    bb.append( i8 );
    bb.append( c  );
    bb.append( string{" str "} );
    bb.append( "const char*"   );

    EXPECT_EQ( bb.str(), string{"UIC str const char*"} );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, chops_resize )
{
    vbyte_buffer bb{ "1234567890" };
    bb.chop_front(5);
    EXPECT_EQ( bb.str(), "67890" );
    bb.chop_front(50);
    EXPECT_EQ( bb.str(), "" );

    bb = vbyte_buffer{ "1234567890" };
    bb.chop_back(1);
    EXPECT_EQ( bb.str(), "123456789" );
    bb.chop_back(2);
    EXPECT_EQ( bb.str(), "1234567" );
    bb.chop_back(21111);
    EXPECT_EQ( bb.str(), "" );

    bb = vbyte_buffer{ "1234567890" };
    bb.resize(7);
    EXPECT_EQ( bb.str(), "1234567" );
    bb.resize(7);
    EXPECT_EQ( bb.str(), "1234567" );
    EXPECT_THROW( bb.resize(8), std::length_error );
    EXPECT_EQ( bb.str(), "1234567" );
    bb.resize(0);
    EXPECT_EQ( bb.str(), "" );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, left_right_middle )
{
    vbyte_buffer bb{ "1234567890" };
    EXPECT_EQ( bb.left(0).str(), "" );
    EXPECT_EQ( bb.left(3).str(), "123" );
    EXPECT_EQ( bb.left(7).str(), "1234567" );
    EXPECT_EQ( bb.left(9).str(), "123456789" );
    EXPECT_EQ( bb.left(10).str(), "1234567890" );
    EXPECT_EQ( bb.left(11).str(), "1234567890" );
    EXPECT_EQ( bb.left(100).str(), "1234567890" );

    EXPECT_EQ( bb.right(0).str(), "" );
    EXPECT_EQ( bb.right(3).str(), "890" );
    EXPECT_EQ( bb.right(7).str(), "4567890" );
    EXPECT_EQ( bb.right(9).str(), "234567890" );
    EXPECT_EQ( bb.right(10).str(), "1234567890" );
    EXPECT_EQ( bb.right(11).str(), "1234567890" );
    EXPECT_EQ( bb.right(100).str(), "1234567890" );

    EXPECT_EQ( bb.middle(0).str(), "1234567890" );
    EXPECT_EQ( bb.middle(1).str(), "234567890" );
    EXPECT_EQ( bb.middle(1, 3).str(), "234" );
    EXPECT_EQ( bb.middle(1, 8).str(), "23456789" );
    EXPECT_EQ( bb.middle(1, 9).str(), "234567890" );
    EXPECT_EQ( bb.middle(1, 10).str(), "234567890" );
    EXPECT_EQ( bb.middle(9, 10).str(), "0" );
    EXPECT_EQ( bb.middle(10,10).str(), "" );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, starts_ends_with )
{
    vbyte_buffer bb{ "1234567890" };

    EXPECT_TRUE ( bb.starts_with("123") );
    EXPECT_FALSE( bb.starts_with("23") );
    EXPECT_TRUE ( bb.starts_with("1234567890") );
    EXPECT_FALSE( bb.starts_with("12345678900") );

    EXPECT_TRUE ( bb.ends_with("890") );
    EXPECT_FALSE( bb.ends_with("89") );
    EXPECT_TRUE ( bb.ends_with("1234567890") );
    EXPECT_FALSE( bb.ends_with("01234567890") );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, text )
{
    EXPECT_EQ( vbyte_buffer("12345").to_int(), 12345 );
    EXPECT_EQ( vbyte_buffer("123456").to_uint(), 123456 );

    EXPECT_DOUBLE_EQ( vbyte_buffer("123.45").to_double(), 123.45 );
    EXPECT_FLOAT_EQ( vbyte_buffer("123.456").to_double(), 123.456f );

    EXPECT_EQ( vbyte_buffer("0123456789012345").to_long(), 123456789012345l );
    EXPECT_EQ( vbyte_buffer("012345678901234567 ").to_ulong(), 12345678901234567ul);
}

//=======================================================================================

TEST_F( VByteBuffer_Test, splitters )
{
    //------------------------------------------------------------------
    auto must = vbyte_buffer::vector{};
    auto vec  = vbyte_buffer("").split('|');
    EXPECT_EQ( vec, must );
    vec  = vbyte_buffer("").split_by_spaces();
    EXPECT_EQ( vec, must );
    //------------------------------------------------------------------
    must = vbyte_buffer::vector{ vbyte_buffer() };
    vec  = vbyte_buffer("|").split('|');
    EXPECT_EQ( vec, must );

    must = vbyte_buffer::vector{ vbyte_buffer(""),vbyte_buffer("") };
    vec  = vbyte_buffer("||").split('|');
    EXPECT_EQ( vec, must );

    must.clear();
    vec  = vbyte_buffer(" ").split_by_spaces();
    EXPECT_EQ( vec, vbyte_buffer::vector{} );

    vec  = vbyte_buffer("   \t  \r  \n  \f  ").split_by_spaces();
    EXPECT_EQ( vec, must );
    //------------------------------------------------------------------
    vec  = vbyte_buffer("|1|2||345|").split('|');
    must = vbyte_buffer::vector
        {
            vbyte_buffer(""),
            vbyte_buffer("1"),
            vbyte_buffer("2"),
            vbyte_buffer(""),
            vbyte_buffer("345")
            // has not last empty value.
        };
    EXPECT_EQ( vec, must );
    //------------------------------------------------------------------
    {
        vbyte_buffer bbt{"\t\n \r\f  12345  \n\n\n  \t \r"};
        bbt.trim_spaces();
        EXPECT_EQ( bbt.str(), "12345" );
    }
    {
        vbyte_buffer bbt{"\t\n \r\f  12345  \n\n\n  \t \r"};
        EXPECT_EQ( bbt.trim_spaces().str(), "12345" );
    }
    //------------------------------------------------------------------
    vec = vbyte_buffer("123 \t 456 ").split_by_spaces();
    must = vbyte_buffer::vector
        {
            vbyte_buffer("123"),
            vbyte_buffer("456"),
        };
    EXPECT_EQ( vec, must );
    //------------------------------------------------------------------
}

//=======================================================================================

TEST_F( VByteBuffer_Test, hex )
{
    auto abra = vbyte_buffer::from_hex( "Abracadabra" ).to_Hex();
    EXPECT_EQ( abra.str(), "0A BA CA DA BA" );

    //При нечетном количестве шестнадцатеричных символов, считается, что первый байт
    //обозначен одним символом:
    auto odd_syms = vbyte_buffer::from_hex("123").to_Hex();
    EXPECT_EQ( odd_syms.str(), "01 23" );

    vbyte_buffer test;
    test.append( char(0x05) );
    test.append( char(0x0A) );
    test.append( char(0x1B) );

    auto res = test.tohex();        // "050a1b"
    EXPECT_EQ( res.str(), "050a1b" );

    res = test.toHex();             // "050A1B"
    EXPECT_EQ( res.str(), "050A1B" );

    res = test.to_hex();            // "05 0a 1b"
    EXPECT_EQ( res.str(), "05 0a 1b" );

    res = test.to_Hex();            // "05 0A 1B"
    EXPECT_EQ( res.str(), "05 0A 1B" );

    res = test.to_Hex('#');         // "05#0A#1B"
    EXPECT_EQ( res.str(), "05#0A#1B" );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, append_LE_BE )
{
    char     center = 0x00;
    int32_t  i32    = 0x12345678;
    uint16_t u16    = 0xABCD;

    vbyte_buffer be, le;

    be.append_BE(i32);
    be.append(center);
    be.append_BE(u16);
    auto be_Hex = be.to_Hex();

    le.append_LE(i32);
    le.append_LE(center);
    le.append_LE(u16);
    auto le_Hex = le.to_Hex();

    EXPECT_EQ( be_Hex.str(), "12 34 56 78 00 AB CD" );
    EXPECT_EQ( le_Hex.str(), "78 56 34 12 00 CD AB" );
}

//=======================================================================================

TEST_F( VByteBuffer_Test, simple_view_1 )
{
    const string   s    = "Hello"; // size = 5.
    const int32_t  i32  = 0x12345678;
    const uint16_t u16  = 0xABCD;
    const float    f    = 123.45f;
    const double   d    = 6789.1234456;

    vbyte_buffer buf;

    buf.append( s );
    buf.append_BE( i32 );
    buf.append_BE( u16 );
    buf.append_BE( f );
    buf.append_BE( d );

    auto view = buf.view();
    auto es = view.string(5);   EXPECT_EQ( es, s );
    auto ei32 = view.i32_BE();  EXPECT_EQ( ei32, i32 );
    auto eu16 = view.u16_BE();  EXPECT_EQ( eu16, u16 );
    auto ef = view.float_BE();  EXPECT_EQ( ef, f );
    auto ed = view.double_BE(); EXPECT_EQ( ed, d );
    EXPECT_TRUE( view.finished() );
    EXPECT_TRUE( view.remained() == 0 );
    EXPECT_TRUE( buf.starts_with("Hello") );
    EXPECT_EQ( buf.middle(5,4).to_Hex().str(), "12 34 56 78" );

    buf.clear();

    buf.append( s );
    buf.append_LE( i32 );
    buf.append_LE( u16 );
    buf.append_LE( f );
    buf.append_LE( d );

    view = buf.view();
    es = view.string(5);   EXPECT_EQ( es, s );
    ei32 = view.i32_LE();  EXPECT_EQ( ei32, i32 );
    eu16 = view.u16_LE();  EXPECT_EQ( eu16, u16 );
    ef = view.float_LE();  EXPECT_EQ( ef, f );
    ed = view.double_LE(); EXPECT_EQ( ed, d );
    EXPECT_TRUE( view.finished() );
    EXPECT_TRUE( view.remained() == 0 );
    EXPECT_TRUE( buf.starts_with("Hello") );
    EXPECT_EQ( buf.middle(5,4).to_Hex().str(), "78 56 34 12" );
}
//=======================================================================================

TEST_F( VByteBuffer_Test, simple_view_omit )
{
    auto str = "1234";
    {
        vbyte_buffer_view b( str, 4 );
        b.omit( 2 );
        EXPECT_EQ( b.remained(), 2 );
        EXPECT_EQ( b.show_tail().str(), "34" );
    }
    {
        vbyte_buffer_view b( str, 4 );
        b.omit( 200000 ); // не должно сломаться.
        EXPECT_EQ( b.remained(), 0 );
        EXPECT_EQ( b.show_tail().str(), "" );
    }
}
//=======================================================================================

TEST_F( VByteBuffer_Test, datas )
{
    vbyte_buffer buf;
    buf.append_LE(42);

    auto c1 = buf.data();
    auto u1 = buf.udata();
    auto s1 = buf.sdata();

    auto view = buf.view();
    auto c2 = view.data();
    auto u2 = view.udata();
    auto s2 = view.sdata();

    EXPECT_EQ( c1, c2 );
    EXPECT_EQ( u1, u2 );
    EXPECT_EQ( s1, s2 );
}
//=======================================================================================

TEST_F( VByteBuffer_Test, pluses )
{
    vbyte_buffer b1("2+");
    vbyte_buffer b2("2=4");

    auto sum = b1 + b2;
    b1 += b2;
    EXPECT_EQ( sum.str(), "2+2=4" );
    EXPECT_EQ( b1.str(),  "2+2=4" );
}
//=======================================================================================

TEST_F( VByteBuffer_Test, implicit_ctor )
{
    auto l = [](vbyte_buffer){};

    //  Тест что можно передавать без явного преобразования.
    l("Hello");
}
//=======================================================================================

TEST_F( VByteBuffer_Test, empty )
{
    vbyte_buffer empty;
    vbyte_buffer not_empty( "Hello" );

    EXPECT_TRUE  ( empty.empty()     );
    EXPECT_FALSE ( not_empty.empty() );
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
