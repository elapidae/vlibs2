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

    static void add_shared_log( std::string fname,
                                uint bytes_in_one,
                                uint rotates );

    static void add_leveled_log( std::string path,
                                 uint bytes_in_one,
                                 uint rotates );

    //-----------------------------------------------------------------------------------
    //  Кухня работы с доменами логов.
    static void omit_domain ( const std::string& domain );  // перестаем логировать.
    static void apply_domain( const std::string& domain );  // будем логировать.

    //-----------------------------------------------------------------------------------
    //  Utils
    static std::string base_name( const char* file );

    //-----------------------------------------------------------------------------------
private:
    //  Вызывается только из logger, error.
    friend class impl_vlog::logger;
    friend class impl_vlog::error;
    static void _execute( const impl_vlog::entry& e );

}; // vlog class
//=======================================================================================

#include "impl_vlog/vlog_logger.h"
#include "impl_vlog/vlog_error.h"

//=======================================================================================

#endif // VLOG_H
