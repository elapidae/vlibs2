#ifndef VLOG_IMPL_LOGGER_H
#define VLOG_IMPL_LOGGER_H

#include "vcat_impl/vcat_iface.h"
#include "vlog_impl/log_entry.h"
#include <functional>

//=======================================================================================
namespace vlog_impl
{
    //===================================================================================
    class logger final : public impl::vcat_iface<logger>
    {
    public:
        //-------------------------------------------------------------------------------

        //  У класса интерфейс vcat (см. заголовок vcat.h).

        log_entry entry()       const;
        operator log_entry()    const;

    //-----------------------------------------------------------------------------------
    //  Кухня настройки логгирования.

    using Executer = std::function< void(const log_entry& entry) >;

    static void add_executer( Executer e );
    static void clear_executers();
    static void execute( const log_entry& entry );

    //  Выводят в виде "[file.cpp:123] DBG message\n";
    static void add_to_cout();
    static void add_to_cerr();
    static void add_to_cout_and_cerr(); // warnings & fatals -> cerr

    //  Иногда надо, НО! Не злоупотребляйте!
    static void add_to_cout_mutexed();
    static void add_to_cerr_mutexed();
    static void add_to_cout_and_cerr_mutexed();

    //-----------------------------------------------------------------------------------
    //  Внутренная кухня.

    //  Предназначен для создания исключительно из макросов.
    enum class _is_proxy { is_proxy, no_proxy };
    logger( log_entry::Level     level,
             const char*          file,
             int32_t              line,
             const char*          func,
             _is_proxy            proxy );

    ~logger();

    logger& operator[](bool on);
    logger& operator[](std::string domain);

    //-----------------------------------------------------------------------------------
private:
    bool _is_on = true;

    vtime_point         _stamp;
    log_entry::Level    _level;
    const char*         _file;
    int32_t             _line;
    const char*         _func;
    bool                _i_am_proxy;

    std::stringstream   _stream;

    friend class VError;
    friend class impl::vcat_iface<logger>;
    template<typename T> void do_cat( T&& val )
    {
        if ( !_is_on ) return;
        _stream << std::forward<T>(val);
    }

    logger( logger && )                   = delete;
    logger( const logger & )              = delete;
    logger& operator = ( logger && )      = delete;
    logger& operator = ( const logger & ) = delete;
};
//=======================================================================================

} // namespace impl

#endif // VLOG_IMPL_LOGGER_H
