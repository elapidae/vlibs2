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
#include "vlog.h"
#include "vtimer.h"
#include "vapplication.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
class VTimer_Test: public testing::Test
{};
#pragma GCC diagnostic pop

using namespace std::chrono;

//=======================================================================================

TEST_F( VTimer_Test, 1 )
{
    vtimer t;
    t.singleshot( nanoseconds(1) );

    int check = -1;
    t.timeout += [&]
    {
        check = 42;
        vapplication::stop();
    };
    t.timeout_times += [](uint64_t times)
    {
        vdeb << times;
    };

    vapplication::poll();

    EXPECT_EQ( check, 42 );
}

//=======================================================================================

TEST_F( VTimer_Test, _2 )
{
    #if V_CAN_PROXY_CONSTRUCTORS
        vtimer t( nanoseconds(1) );
    #else
        vtimer t;
        t.start( nanoseconds(1) );
    #endif

    int count = -1;
    t.timeout_times += [&](uint64_t times)
    {
        ++count;
        vdeb << count << "times:" << times;
        if ( count == 5 )
            vapplication::stop();
    };

    vapplication::poll();

    //  Столь странная проверка, если таймер успеет прилететь на поллинг.
    EXPECT_TRUE( count == 5 || count == 6 );
    //-----------------------------------------------------------------------------------
    //  Вторая часть на аккуратную остановку.
    count = -1;
    t.timeout_times.unlink_all();
    t.timeout_times += [&](uint64_t times)
    {
        ++count;
        vdeb << count << "times 2:" << times;
        if ( count == 5 )
        {
            t.stop();   //  Здесь разница между предыдущей частью. Остановленный таймер
                        //  не должен дать еще сигнал.
            vapplication::stop();
        }
    };

    vapplication::poll();

    EXPECT_EQ( count, 5 );
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
