#include "vlog.h"

#include <iostream>
#include <vector>

#include "vcat.h"

using namespace std;


//=======================================================================================
//      vlog::entry
//=======================================================================================
vlog::entry::entry( entry::Level          level,
                    const vtime_point&    stamp,
                    const char*           file,
                    int                   line,
                    const char*           func,
                    const string&         msg,
                    const string&         domain )
    : _level  ( level  )
    , _stamp  ( stamp  )
    , _file   ( file   )
    , _line   ( line   )
    , _func   ( func   )
    , _msg    ( msg    )
    , _domain ( domain )
{}
//=======================================================================================
string vlog::entry::place() const
{
    return vcat( '[', base_name(_file), ':', _line, ']' );
}
//=======================================================================================
string vlog::entry::for_std_cxxx() const
{
    vcat res( place(), ' ', level_str() );

    if ( has_domain() )
        res( " {", _domain, '}' );

    res( ":\t", message() );

    return res;
}
//=======================================================================================
const string& vlog::entry::message() const
{
    return _msg;
}
//=======================================================================================
const vtime_point& vlog::entry::timestamp() const
{
    return _stamp;
}
//=======================================================================================
const char* vlog::entry::filepath() const
{
    return _file;
}
//=======================================================================================
string vlog::entry::filename() const
{
    return base_name( _file );
}
//=======================================================================================
int32_t vlog::entry::line() const
{
    return _line;
}
//=======================================================================================
const char* vlog::entry::function() const
{
    return _func;
}
//=======================================================================================
vlog::entry::Level vlog::entry::level() const
{
    return _level;
}
//=======================================================================================
bool vlog::entry::is_trace() const
{
    return level() == Level::Trace;
}
//=======================================================================================
bool vlog::entry::is_debug() const
{
    return level() == Level::Dbg;
}
//=======================================================================================
bool vlog::entry::is_runlog() const
{
    return level() == Level::Runlog;
}
//=======================================================================================
bool vlog::entry::is_warning() const
{
    return level() == Level::Warning;
}
//=======================================================================================
bool vlog::entry::is_fatal() const
{
    return level() == Level::Fatal;
}
//=======================================================================================
bool vlog::entry::has_domain() const
{
    return !_domain.empty();
}
//=======================================================================================
const string &vlog::entry::domain() const
{
    return _domain;
}
//=======================================================================================
string vlog::entry::level_str() const
{
    switch ( _level )
    {
    case Level::Trace:   return "TRC";
    case Level::Dbg:     return "DBG";
    case Level::Runlog:  return "RUN";
    case Level::Warning: return "WRN";
    case Level::Fatal:   return "FTL";
    }
    throw std::logic_error( "Unknown type" );
}
//=======================================================================================
char vlog::entry::level_char() const
{
    switch ( _level )
    {
    case Level::Trace:    return 'T';
    case Level::Dbg:      return 'D';
    case Level::Runlog:   return 'R';
    case Level::Warning:  return 'W';
    case Level::Fatal:    return 'F';
    }
    throw std::logic_error( "Unknown type" );
}
//=======================================================================================
vlog::entry::Level vlog::entry::level_from_char( char ch )
{
    switch ( ch )
    {
    case 'T': return Level::Trace;
    case 'D': return Level::Dbg;
    case 'R': return Level::Runlog;
    case 'W': return Level::Warning;
    case 'F': return Level::Fatal;
    }
    throw std::logic_error( "Unknown type" );
}
//=======================================================================================
//      vlog::entry
//=======================================================================================
