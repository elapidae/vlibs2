#include "impl_vlog/vlog_logger.h"

#include <cassert>

using namespace std;

//=======================================================================================
//      vlog::logger
//=======================================================================================
vlog::logger::logger( entry::Level  level,
                      const char*   file,
                      int32_t       line,
                      const char*   func )
    : _stamp ( vtime_point::now() )
    , _level ( level )
    , _file  ( file  )
    , _line  ( line  )
    , _func  ( func  )
{
    space();    // Включаем пробелы между аргументами по умолчанию.
}
//=======================================================================================
vlog::logger::~logger()
{
    if ( !_is_on ) return;

    auto msg = _stream.str();
    if ( delimiter_was_added() )
        msg.back() = '\n';
    else
        msg.push_back( '\n' );

    auto ent = entry( _level, _stamp, _file, _line, _func, msg, _domain );
    vlog::_execute( ent );
}
//=======================================================================================
vlog::logger& vlog::logger::operator[]( bool on )
{
    _is_on = on;
    return *this;
}
//=======================================================================================
vlog::logger& vlog::logger::operator[]( const string& domain )
{
    assert( _domain.empty() );
    _domain = domain;
    return *this;
}
//=======================================================================================
vlog::logger& vlog::logger::operator[]( const char* domain )
{
    assert( _domain.empty() );
    _domain = domain;
    return *this;
}
//=======================================================================================
//      vlog::logger
//=======================================================================================
