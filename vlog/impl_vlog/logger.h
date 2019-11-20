#ifndef IMPL_VLOG_LOGGER_H
#define IMPL_VLOG_LOGGER_H

#include "impl_vlog/entry.h"
#include "impl_vcat/vcat_iface.h"

//=======================================================================================
namespace impl_vlog
{
    //===================================================================================
    class logger final : public impl_vcat::vcat_iface<logger>
    {
    public:
        //-------------------------------------------------------------------------------

        //  Предназначен для создания исключительно из макросов.
        logger( position_fix && pos,
                entry::Level    level );

        ~logger();

        logger& operator[] ( bool on );                     // оператор для выключения.

        logger& operator[] ( const std::string& domain );   // логирование по доменам
        logger& operator[] ( const char* domain );          //

        //-------------------------------------------------------------------------------
    private:
        bool _is_on = true;         // меняется через operator[bool]
        std::string _domain = {};   // меняется через operator[string]

        position_fix    _pos;
        entry::Level    _level;

        std::ostringstream _stream;

        friend class impl_vcat::vcat_iface<logger>;
        template<typename T>
        void do_cat( T && val )
        {
            if ( !_is_on ) return;
            _stream << std::forward<T>( val );
        }

        logger( logger && )                   = delete;
        logger( const logger & )              = delete;
        logger& operator= ( logger && )       = delete;
        logger& operator= ( const logger & )  = delete;
    }; // logger class
    //===================================================================================
} // impl_vlog namespace
//=======================================================================================

//=======================================================================================
#define vtrace      impl_vlog::logger( V_POSITION_FIX, impl_vlog::entry::Level::Trace   )

#define vdebug      impl_vlog::logger( V_POSITION_FIX, impl_vlog::entry::Level::Dbg     )
#define vdeb vdebug

#define vrunlog     impl_vlog::logger( V_POSITION_FIX, impl_vlog::entry::Level::Runlog  )

#define vwarning    impl_vlog::logger( V_POSITION_FIX, impl_vlog::entry::Level::Warning )

#define vfatal      impl_vlog::logger( V_POSITION_FIX, impl_vlog::entry::Level::Fatal   )
//=======================================================================================

#endif // IMPL_VLOG_LOGGER_H
