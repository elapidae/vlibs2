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
//      vlog
//=======================================================================================
class vlog
{
public:
    //-----------------------------------------------------------------------------------
    class entry;
    class logger;
    class error;

    //-----------------------------------------------------------------------------------
    //  Кухня настройки логгирования.

    using Executer = std::function< void(const entry&) >;

    static void clear_executers();
    static void add_executer( Executer e );

    //  Добавляет в выполнители канал по cout, cerr.
    //  NB! не контролирует количество вызовов.
    static void add_log_to_cout();
    static void add_log_to_cerr();

    //  not implemented yet...
    static void add_file_shared_log( std::string fname,
                                     uint bytes_in_one,
                                     uint rotates );

    //  not implemented yet...
    static void add_file_leveled_log( std::string path,
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
    //  Вызывается из logger, error.
    static void _execute( const entry& e );
}; // vlog class
//=======================================================================================
//      vlog
//=======================================================================================

#include "impl_vlog/vlog_entry.h"
#include "impl_vlog/vlog_logger.h"
#include "impl_vlog/vlog_error.h"

//=======================================================================================
//      defines
//=======================================================================================
#define VLOG_FUNCTION __PRETTY_FUNCTION__

#define vtrace          vlog::logger( vlog::entry::Level::Trace,        \
                                      __FILE__, __LINE__, VLOG_FUNCTION )

#define vdebug          vlog::logger( vlog::entry::Level::Dbg,          \
                                      __FILE__, __LINE__, VLOG_FUNCTION )
#define vdeb vdebug

#define vrunlog         vlog::logger( vlog::entry::Level::Runlog,       \
                                      __FILE__, __LINE__, VLOG_FUNCTION )

#define vwarning        vlog::logger( vlog::entry::Level::Warning,      \
                                      __FILE__, __LINE__, VLOG_FUNCTION )

#define vfatal          vlog::logger( vlog::entry::Level::Fatal,        \
                                      __FILE__, __LINE__, VLOG_FUNCTION )

#define verror vlog::error( __FILE__, __LINE__, VLOG_FUNCTION )
//=======================================================================================
//      defines
//=======================================================================================


#endif // VLOG_H
