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
#include "gtest/gtest.h"

#include "vlog.h"

class VLog_Test: public testing::Test
{};

using namespace std;

template<class> class TD;

//=======================================================================================
//  Пример-затравка для пользовательской обработки логов.
void my_log_executer( const vlog::entry &entry )
{
    // Ниже приведены содержания из точки логгирования.
    // Их следует использовать для составления собственных сообщений.

    entry.timestamp();  // Метка времени создания вхождения.

    entry.level_char(); // Однобуквенный код типа сообщения { T, D, R, W, F }.
    entry.level_str();  // Трехбуквенный код типа сообщения { TRC, DBG, RUN, WRN, FLT }.
    entry.level();      // Тип enum class vlog::vlog::entry::Type.

    entry.filename();   // Имя файла без пути до него.
    entry.filepath();   // Полное имя файла (то, что дает __FILE__).
    entry.line();       // Номер строки в исходнике __LINE__
    entry.function();   // Что дает __PRETTY_FUNCTION__.

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
    vtrace   << "alla";
    vdeb     << "alla";
    vrunlog  << "alla";
    vwarning << "alla";
    vfatal   << "alla";
}

//=======================================================================================

TEST_F( VLog_Test, _error )
{
    EXPECT_THROW( throw verror << "Hello world!", vlog::error );
}

//=======================================================================================

TEST_F( VLog_Test, _1 )
{
    // По умолчанию будет выводить в консоль.
    vrunlog << "Hello World!"
            << "sizeof(vlog::entry):"   << sizeof(vlog::entry)
            << "sizeof(vtime_point):" << sizeof(vtime_point)
            << "sizeof(string):"      << sizeof(string);

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
    vdeb.num(42, 5, '0');                     // то же самое, 00042
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
    vlog::add_executer( [&](const vlog::entry&)
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
//TEST_F( VLog_Test, _2 )
//{
//    // Удалим текущие логгеры и будем писать только в файлы.
//    VLogger::clear_executers();

//    // Будем вести историю максимум в двух файлах, размеры одного -- 2.5 кб.
//    VFileLog_Shared shared_flog( vcat("shared-log.log"), 2500, 2 );
//    shared_flog.register_self(); // Он сам знает где и как регистрироваться.

//    // Будем вести историю максимум в двух файлах, размеры одного -- 1 кб.
//    VFileLog_Leveled leveled_flog( "./logs", 1000, 2 );
//    leveled_flog.register_self();

//    for (int i = 0; i < 10; ++i)
//    {
//        string msg = vcat("Testing records in file: ", i)
//                         (", timestamp ms = ", VTimePoint::now().milliseconds());
//        vtrace   (msg);
//        vdeb   (msg);
//        vrunlog  (msg);
//        vwarning (msg);
//        vfatal   (msg);
//    }
//    // См. логи около бинарника программы.
//}
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
