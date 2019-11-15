#ifndef IMPL_VLOG_LOGGER_H
#define IMPL_VLOG_LOGGER_H

#include "vlog.h"
#include "impl_vcat/vcat_iface.h"

//=======================================================================================
//      vlog::logger
//=======================================================================================
class vlog::logger final : public impl_vcat::vcat_iface<vlog::logger>
{
public:
    //-----------------------------------------------------------------------------------

    //  Предназначен для создания исключительно из макросов.
    logger( entry::Level    level,
            const char*     file,
            int32_t         line,
            const char*     func );

    ~logger();

    logger& operator[] ( bool on );
    logger& operator[] ( const std::string& domain );

    //-----------------------------------------------------------------------------------
private:
    bool _is_on = true;         // меняется через operator[bool]
    std::string _domain = {};   // меняется через operator[string]

    vtime_point     _stamp;
    entry::Level    _level;
    const char*     _file;
    int32_t         _line;
    const char*     _func;

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

    //===================================================================================
};
//=======================================================================================
//      vlog::logger
//=======================================================================================


#endif // IMPL_VLOG_LOGGER_H
