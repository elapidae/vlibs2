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

class VLog_Test: public testing::Test
{};

using namespace std;

template<class> class TD;

//=======================================================================================
//  Пример-затравка для пользовательской обработки логов.
void my_log_executer( const impl_vlog::entry &entry )
{
    // Ниже приведены содержания из точки логгирования.
    // Их следует использовать для составления собственных сообщений.

    entry.pos().stamp();  // Метка времени создания вхождения.

    entry.level_char(); // Однобуквенный код типа сообщения { T, D, R, W, F }.
    entry.level_str();  // Трехбуквенный код типа сообщения { TRC, DBG, RUN, WRN, FLT }.
    entry.level();      // Тип enum class vlog::vlog::entry::Type.

    entry.pos().filename();   // Имя файла без пути до него.
    entry.pos().filepath();   // Полное имя файла (то, что дает __FILE__).
    entry.pos().line();       // Номер строки в исходнике __LINE__
    entry.pos().function();   // Что дает __PRETTY_FUNCTION__.

    entry.message();    //  Составленное сообщение.

    entry.is_trace();   //  Для быстрой проверки к-л уровня.
    entry.is_debug();   //
    entry.is_runlog();  //
    entry.is_warning(); //
    entry.is_fatal();   //

    cout << "my_log_executer: " << entry.message() << endl;
}
//=======================================================================================

//=======================================================================================

TEST_F( VLog_Test, _alla )
{
    int a = 0;
    float b = 7.6;
    std::unordered_map<int, float> map;
    map.emplace(9, 2.78);
    map.emplace(91, 2.78);
    map.emplace(95, 2.78);
    vdeb("bebe", a, b, map);

    vtrace   << "alla";
    vdeb     << "alla";
    vrunlog  << "alla";
    vwarning << "alla";
    vfatal   << "alla";
}

//=======================================================================================

TEST_F( VLog_Test, _error )
{
    EXPECT_THROW( throw verror << "Hello world!", impl_vlog::error );
}

//=======================================================================================

TEST_F( VLog_Test, _1 )
{
    // По умолчанию будет выводить в консоль.
    vrunlog << "Hello World!"
            << "sizeof(vlog::entry):"   << sizeof(impl_vlog::entry)
            << ", sizeof(vtime_point):" << sizeof(vtime_point)
            << ", sizeof(string):"      << sizeof(string);

    //  Вводные примеры, определим пару простых переменных и плюнем ими в консоль.
    double dd = 3.1415;
    float  ff = 2.718f;
    int    ii = 42;
    std::chrono::milliseconds ms(12345); // и такое выводим`c...

    vdeb; // пустая строка
    vdeb << "------- same syntax example.";
    vdeb << dd << ff << ii << ms;   // Одно
    vdeb(dd, ff, ii, ms);           // и то
    vdeb(dd)(ff)(ii)(ms);           // же.

    // Примеры с манипуляторами вывода потока.
    double long_term_val = 1.0 / 3.0; // Значение с большим кол-вом знаков после запятой.
    vdeb << "------ precision example.";
    vdeb << long_term_val;                    // выведет мало знаков.
    vdeb.max_precision() << long_term_val;    // выведет максимальное кол-во знаков.

    vdeb << "------ fill & width example.";
    vdeb.fill_char('0').field_width(5) << 42; // 00042
    vdeb.aligned(42, 5, '0');                 // то же самое, 00042
                                              // специально заточено под эту задачу.
    vdeb << "------------------------------";

    // Вывод без пробелов между аргументами:
    string prog_name = "some-dir/some-file.exe";
    vtrace.nospace()( "filename is '", prog_name, "'." );

    // Теперь будем логгировать в cerr, удалим всех исполнителей и добавим исполнитель,
    // который будет писать в cerr.
    vlog::clear_executers();
    vlog::add_log_to_cerr();
    vrunlog << "Hello World in cerr!";

    // регистрируем своего исполнителя.
    vlog::clear_executers();
    vlog::add_executer( my_log_executer );
    vwarning("After register own executer.");
}

//=======================================================================================

TEST_F( VLog_Test, test_on_off )
{
    vlog::clear_executers();

    int count = 0;
    vlog::add_executer( [&](const impl_vlog::entry&)
    {
        ++count;
    });

    vtrace << 1;
    vtrace << 2;
    vtrace << 3;

    vtrace[false]   << 3;
    vdeb[false]     << 3;
    vrunlog[false]  << 3;
    vwarning[false] << 3;
    vfatal[false]   << 3;

    vtrace[true]    << 4;

    EXPECT_EQ( count, 4 );
}

//=======================================================================================

TEST_F( VLog_Test, test_domain )
{
    vlog::clear_executers();
    vlog::add_log_to_cout();

    string d;
    vlog::add_executer( [&](const impl_vlog::entry& e)
    {
        d = e.domain();
    });

    vtrace["domain"] << 1;

    EXPECT_EQ( d, "domain" );
}

//=======================================================================================

TEST_F( VLog_Test, examples )
{
    vlog::clear_executers();
    vlog::add_log_to_cout();

    vtrace << "frame received" << 42;
    vdeb << "debug example";
    vrunlog << "socket opened";
    vwarning << "socket closed";
    vfatal << "server killed";

    string buffer;

    bool need_trace_frames_receiving = true;

    if ( need_trace_frames_receiving )
        vtrace << "frame received";

    vtrace[need_trace_frames_receiving] << "frame received";

    vlog::omit_domain( "DOMAIN1" );
    vdeb["DOMAIN1"] << 42;
    vdeb << 42;
    vdeb["DOMAIN_2"][true] << 42;
    vdeb[false]["DOMAIN_2"] << 42;
}

//=======================================================================================
TEST_F( VLog_Test, _primitive_file_logs )
{
    // Удалим текущие логгеры и будем писать только в файлы.
    vlog::clear_executers();
    vlog::set_shared_log ( "test.log",  1000, 2 );
    vlog::set_leveled_log( "test_logs", 1000, 2 );

    for (int i = 0; i < 10; ++i)
    {
        string msg = vcat("Testing records in file: ", i)
                         (", timestamp ms = ", vtime_point::now());
        vtrace   (msg);
        vdeb     (msg);
        vrunlog  (msg);
        vwarning (msg);
        vfatal   (msg);
    }
    // См. логи около бинарника программы.
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
