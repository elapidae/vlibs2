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

#ifndef VLOG_H
#define VLOG_H

//=======================================================================================
/*
 *  2019-11-19      by elapidae
 *
 *  Вторая версия системы логирования.
 *
 *  Все методы статические. Все сделано так, что API должно покрыть ~100% для разработки
 *  микросервисов в НИИАС-е.
 *
 *  По сравнению с предыдущей версией, обзавелась доменами и флагами выключения.
 *
 *  NB! По умолчанию, сообщения выводятся через cout. Не рекомендую разделять вывод
 *      между cout и cerr, т.к. они по разному синхронизируются. По этой же причине из
 *      коробки не сделано такого метода вывода.
 *
 *
 *  static void clear_executers();
 *      Удаляет все логеры.
 *
 *  static void add_executer( Executer e );
 *      Вставляет кастомный логер.
 *
 *  static void add_log_to_cout();
 *  static void add_log_to_cerr();
 *      Добавляет в выполнители канал по cout, cerr.
 *      NB! не контролирует количество вызовов.
 *
 *  static void set_shared_log( const std::string& fname,
 *                              uint bytes_in_one,
 *                              uint rotates );
 *      Устанавливает штатный shared логер. Заменяет предыдущий.
 *
 *  static void set_leveled_log( const std::string& path,
 *                               uint bytes_in_one,
 *                               uint rotates );
 *      Устанавливает штатный leveled логер. Заменяет предыдущий.
 *
 *  Логирование по доменам:
 *  static void omit_domain ( const std::string& domain );  // перестаем логировать.
 *  static void apply_domain( const std::string& domain );  // будем логировать.
*/
//=======================================================================================

#include <string>
#include <stdexcept>
#include <functional>

//=======================================================================================
namespace impl_vlog
{
    class entry;
    class logger;
    class error;
}
//=======================================================================================
class vlog
{
public:
    //-----------------------------------------------------------------------------------
    //  Кухня настройки логгирования.

    using Executer = std::function< void(const impl_vlog::entry&) >;

    static void clear_executers();
    static void add_executer( Executer e );

    //  Добавляет в выполнители канал по cout, cerr.
    //  NB! не контролирует количество вызовов.
    static void add_log_to_cout();
    static void add_log_to_cerr();

    static void set_shared_log( const std::string& fname,
                                uint bytes_in_one,
                                uint rotates );

    static void set_leveled_log( const std::string& path,
                                 uint bytes_in_one,
                                 uint rotates );

    //-----------------------------------------------------------------------------------
    //  Кухня работы с доменами логов.
    static void omit_domain ( const std::string& domain );  // перестаем логировать.
    static void apply_domain( const std::string& domain );  // будем логировать.

    //-----------------------------------------------------------------------------------
private:
    //  Вызывается только из logger, error.
    friend class impl_vlog::logger;
    friend class impl_vlog::error;
    static void _execute( const impl_vlog::entry& e );

}; // vlog class
//=======================================================================================

#include "impl_vlog/logger.h"
#include "impl_vlog/error.h"

//=======================================================================================

#endif // VLOG_H
