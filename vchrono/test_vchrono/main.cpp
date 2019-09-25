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


#include <iostream>
#include <string.h>

#include "gtest/gtest.h"
#include "vtime_point.h"
#include "vtime_meter.h"

//#include "vtimemeter.h"
//#include <list>
//#include <deque>

using namespace std;
using namespace std::chrono;


//=======================================================================================
#define vdeb std::cout << basename(__FILE__) << ":" << __LINE__ << "==> "
//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VChrono_Test: public testing::Test {};
#pragma GCC diagnostic pop
//=======================================================================================
template<class> class TD;


//=======================================================================================
//TEST_F( VChrono_Test, set_random )
//{
//    srandom( uint(VTimePoint::now().microseconds().count()) );
//}
//=======================================================================================
template<typename time_point>
static void steady_test(string who)
{
    vdeb << who << " is"
         << (time_point::is_steady() ? " " : " not ")
         << "steady." << endl;
}

TEST_F( VChrono_Test, steady_test )
{
    steady_test<vtime_point>("vtime_point");
    steady_test<vsteady_time_point>("vsteady_time_point");
    steady_test<vhigh_resolution_time_point>("vhigh_resolution_time_point");
}
//=======================================================================================
template<typename time_point>
static void base_test(string who)
{
    time_point ctor1;
    time_point ctor2( time_point::system_type::clock::now() );

    //TD<decltype(time_point::system_type::clock::now())> fl;
    auto p1 = vtime_point::now();
    vdeb << "=====================" << endl;
    vdeb << who << endl;
    vdeb << p1 << endl;
    vdeb << p1.nanoseconds() << endl;
    vdeb << p1.microseconds() << endl;
    vdeb << p1.milliseconds() << endl;
    vdeb << p1.seconds() << endl;
    vdeb << p1.minutes() << endl;
    vdeb << p1.hours() << endl;
}

TEST_F( VChrono_Test, base_test )
{
    base_test<vtime_point>("vtime_point");
    base_test<vsteady_time_point>("vsteady_time_point");
    base_test<vhigh_resolution_time_point>("vhigh_resolution_time_point");
}
//=======================================================================================
template<typename time_point>
static void pluses()
{
    auto p1 = time_point::now();
    usleep(1);
    auto p2 = time_point::now();

    auto ttp1 = p1.time_point();
    auto ttp2 = p2.time_point();
    auto tp1 = ttp1 + ttp2.time_since_epoch();
    auto tp2 = ttp2.time_since_epoch() + ttp1;
    auto plus = p2 + p1.time_point().time_since_epoch();
    EXPECT_EQ( tp1, tp2 );
    EXPECT_EQ( tp1, plus.time_point() );
    EXPECT_EQ( tp2, plus.time_point() );

    p2 += p1.time_point().time_since_epoch();
    EXPECT_EQ( plus, p2 );
}

TEST_F( VChrono_Test, pluses )
{
    pluses<vtime_point>();
    pluses<vsteady_time_point>();
    pluses<vhigh_resolution_time_point>();
}
//=======================================================================================
template<typename time_point>
static void minuses()
{
    auto p1 = time_point::now();
    usleep(1);
    auto p2 = time_point::now();

    auto tp = p2.time_point() - p1.time_point();
    auto minus = p2 - p1;
    EXPECT_EQ( minus, tp );

    minus -= seconds(10);
    EXPECT_EQ( minus, tp - seconds(10) );
}

TEST_F( VChrono_Test, minuses )
{
    minuses<vtime_point>();
    minuses<vsteady_time_point>();
    minuses<vhigh_resolution_time_point>();
}
//=======================================================================================
template<typename time_point>
static void compares()
{
    auto p1 = time_point::now();
    usleep(1);
    auto p2 = time_point::now();

    EXPECT_TRUE( p2 >  p1 );
    EXPECT_TRUE( p2 >= p1 );

    EXPECT_TRUE( p1 <  p2 );
    EXPECT_TRUE( p1 <= p2 );

    EXPECT_TRUE ( p2 != p1 );
    EXPECT_FALSE( p2 == p1 );
}

TEST_F( VChrono_Test, compares )
{
    compares<vtime_point>();
    compares<vsteady_time_point>();
    compares<vhigh_resolution_time_point>();
}

//=======================================================================================

template<typename time_point>
static void humanable_1()
{
    auto p1 = vtime_point::from_date_time("2019-09-25 17:36:15");
    EXPECT_EQ( p1.humanable().year(),        2019   );
    EXPECT_EQ( p1.humanable().month(),       9      );
    EXPECT_EQ( p1.humanable().day(),         25     );
    EXPECT_EQ( p1.humanable().hour(),        17     );
    EXPECT_EQ( p1.humanable().minute(),      36     );
    EXPECT_EQ( p1.humanable().second(),      15     );
    EXPECT_EQ( p1.humanable().millisecond(), 0      );
}

TEST_F( VChrono_Test, humanable_1 )
{
    humanable_1<vtime_point>();
    humanable_1<vsteady_time_point>();
    humanable_1<vhigh_resolution_time_point>();
}

//=======================================================================================

template<typename time_point>
static void humanable_2()
{
    auto pdt  = vtime_point::from_date_time( "2019-09-25 17:36:15" );
    auto piso = vtime_point::from_iso8601 ( "2019-09-25T17:36:15" );
    auto putc = vtime_point::from_utc     (  2019, 9,25,17,36,15  );

    EXPECT_EQ( pdt,  piso );
    EXPECT_EQ( piso, putc );
}

TEST_F( VChrono_Test, humanable_2 )
{
    humanable_2<vtime_point>();
    humanable_2<vsteady_time_point>();
    humanable_2<vhigh_resolution_time_point>();
}

//=======================================================================================

TEST_F( VChrono_Test, simple_time_meter )
{
    vtime_meter meter( vtime_meter::stopped );
    usleep(1);
    EXPECT_EQ( meter.restart().ns(), nanoseconds(0) );

    usleep(1);
    vdeb << "after waited 1 us:" << endl;
    vdeb << meter.elapsed().ns() << endl;
    vdeb << meter.elapsed().us() << endl;
    vdeb << meter.elapsed().ms() << endl;
    vdeb << meter.elapsed().sec() << endl;
    vdeb << meter.elapsed().ns() << endl;
    vdeb << meter.elapsed().us() << endl;
    vdeb << meter.elapsed().ms() << endl;
    vdeb << meter.elapsed().sec() << endl;
}

//TEST_F( VChrono_Test, dev_meter_reverse )
//{
//    VTimeMeter ptr_meter( VTimeMeter::Stopped );
//    VTimeMeter std_meter( VTimeMeter::Stopped );
//    VTimeMeter str_meter( VTimeMeter::Stopped );

//    int circles = 1e6;
//    for ( int i = 0; i < circles; ++i )
//    {
//        float  f = i * 13.5f;
//        double d = i * 123.8;
//        uint64_t u64 = uint64_t(i);
//        int16_t  i16 = rand();

//        auto f1 = f;
//        auto d1 = d;
//        auto u1 = u64;
//        auto i1 = i16;

//        std_meter.unpause();
//            f1 = reverse_as_std( f1 );
//            d1 = reverse_as_std( d1 );
//            u1 = reverse_as_std( u1 );
//            i1 = reverse_as_std( i1 );
//        std_meter.pause();

//        ptr_meter.unpause();
//            f1 = reverse_as_ptr( f1 );
//            d1 = reverse_as_ptr( d1 );
//            u1 = reverse_as_ptr( u1 );
//            i1 = reverse_as_ptr( i1 );
//        ptr_meter.pause();

//        EXPECT_EQ( f,   f1 );
//        EXPECT_EQ( d,   d1 );
//        EXPECT_EQ( u64, u1 );
//        EXPECT_EQ( i16, i1 );

//        VString str;
//        str.append_BE(f1)
//           .append_BE(d1)
//           .append_BE(u1)
//           .append_BE(i1);
//        auto view = str.forward_view();

//        str_meter.unpause();
//            f   = view.take_float_BE();
//            d   = view.take_double_BE();
//            u64 = view.take_u64_BE();
//            i16 = view.take_i16_BE();
//        str_meter.pause();

//        EXPECT_EQ( f,   f1 );
//        EXPECT_EQ( d,   d1 );
//        EXPECT_EQ( u64, u1 );
//        EXPECT_EQ( i16, i1 );
//    }
//    vdeb <<   "PTR TIME:" << ptr_meter.elapsed()
//         << ", STD TIME:" << std_meter.elapsed()
//         << ", CUR STR TIME:" << str_meter.elapsed();
//}
//=======================================================================================
//TEST_F( VChrono_Test, creations )
//{
//    (void)VTimePoint();
//    (void)VSystemTimePoint();
//    (void)VSteadyTimePoint();
//    (void)VHighResolutionTimePoint();
//}
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
