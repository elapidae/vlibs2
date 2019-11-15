#ifndef IMPL_VLOG_VLOG_ENTRY_H
#define IMPL_VLOG_VLOG_ENTRY_H

#include "vlog.h"
#include "vtime_point.h"

//=======================================================================================
//      vlog::entry
//=======================================================================================
// [1]: 3-х символьное обозначение уровня: TRC, DBG, RUN, WRN, FLT;
//      Односимвольный код уровня: T, D, R, W, F.
//      Символьные обозначения применяются при записи логов, чтобы размеры поля
//      были одинаковыми. Также применяется при сериализации (гораздо проще тащить один
//      символ с очевидным значением).
//=======================================================================================
class vlog::entry final
{
public:
    //-----------------------------------------------------------------------------------
    enum class Level { Trace, Dbg, Runlog, Warning, Fatal };

    //-----------------------------------------------------------------------------------
    const std::string& message()    const;

    const vtime_point& timestamp()  const;

    const char* filepath()          const;  //  полное имя файла, обычно ведет
                                            //  к build-папке.
    std::string filename()          const;  //  только имя файла (путь отсечен).

    int32_t     line()              const;

    const char* function()          const;  //  Значение макроса VLOG_FUNCTION.

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

    //-----------------------------------------------------------------------------------
    //  Сервисная часть, нормальным людям ни к чему.

    //  Предназначен для использования внутри других механизмов, конечному
    //  пользователю конструировать этот объект не нужно.
    entry( Level                level,
           const vtime_point&   stamp,
           const char*          file,
           int                  line,
           const char*          func,
           const std::string&   msg,
           const std::string&   domain );

    //-----------------------------------------------------------------------------------
    //  Здесь собраны более-менее стандартные способы представления вхождения в виде
    //  записи в журнале.
    //  В обозначениях:
    //  place       --  [filename:123]
    //  level       --  level_str() == TRC, DBG, RUN, WRN, FLT
    //  time        --  yyyy-MM-ddThh:mm:ss.zzz
    //  msg         --  message
    //  nl          --  new line (\n)
    //  Возвращает конструкции вида:
    std::string place()             const;
    //std::string place_func()        const;  //  "[filename.cpp:123:function]"
    //std::string place_level_msg()           const;
    //std::string time_place_msg_nl()         const;
    //std::string time_place_level_msg_nl()   const;

    //  Сообщение для консольных выводов.
    std::string for_std_cxxx() const;

    // Линия вида "======= BEGIN LOGGING ========= yyyy-MM-dd hh:mm:ss.zzz ======".
    static std::string begin_line_time_nl();

    //-----------------------------------------------------------------------------------
    //  Обратная функция к level_char(). Нужно при десериализации.
    static Level level_from_char( char ch );

    //-----------------------------------------------------------------------------------
private:
    Level           _level;
    vtime_point     _stamp;
    const char*     _file;
    int             _line;
    const char*     _func;
    std::string     _msg;
    std::string     _domain;
}; // vlog::entry
//=======================================================================================
//      vlog::entry
//=======================================================================================

#endif // IMPL_VLOG_VLOG_ENTRY_H
