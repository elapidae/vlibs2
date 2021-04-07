#ifndef IMPL_VLOG_ENTRY_H
#define IMPL_VLOG_ENTRY_H

#include <map>
#include "impl_vlog/position_fix.h"

//=======================================================================================
//      vlog::entry
//=======================================================================================
// [1]: 3-х символьное обозначение уровня: TRC, DBG, RUN, WRN, FLT;
//      Односимвольный код уровня: T, D, R, W, F.
//      Символьные обозначения применяются при записи логов, чтобы размеры поля
//      были одинаковыми. Также применяется при сериализации (гораздо проще тащить один
//      символ с очевидным значением).
//=======================================================================================
namespace impl_vlog
{
    class logger;

    class entry final
    {
    public:
        //-------------------------------------------------------------------------------
        enum class Level { Trace, Dbg, Runlog, Warning, Fatal };
        using fields_type = std::map<std::string,std::string>;
        //-------------------------------------------------------------------------------
        const position_fix& pos()       const;

        const std::string&  message()   const;

        Level       level()             const;  //  См. [1]:
        std::string level_str()         const;  //  3-х символьное обозначение уровня.
        char        level_char()        const;  //  Односимвольный код уровня.

        bool        is_trace()          const;
        bool        is_debug()          const;
        bool        is_runlog()         const;
        bool        is_warning()        const;
        bool        is_fatal()          const;

        bool        has_domain()        const;
        const std::string& domain()     const;

        const fields_type& fields()     const;

        //-------------------------------------------------------------------------------
        //  Сервисная часть, нормальным людям ни к чему.

        //  Предназначен для использования внутри других механизмов, конечному
        //  пользователю конструировать этот объект не нужно.
        entry( const position_fix&  pos,
               Level                level,
               const std::string&   msg,
               const std::string&   domain );

        //-------------------------------------------------------------------------------
        //  Обратная функция к level_char(). Нужно при десериализации.
        static Level level_from_char( char ch );

        //-------------------------------------------------------------------------------
    private:
        position_fix    _pos;
        Level           _level;
        std::string     _msg;
        std::string     _domain;

        friend class    logger;
        fields_type     _fields;
    }; // vlog::entry
    //===================================================================================
} // namespace impl_vlog
//=======================================================================================

#endif // IMPL_VLOG_ENTRY_H
