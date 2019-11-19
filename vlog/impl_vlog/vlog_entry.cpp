#include "vlog.h"

#include <iostream>
#include <vector>

#include "vcat.h"

using namespace impl_vlog;
using namespace std;


//=======================================================================================
//      vlog::entry
//=======================================================================================
entry::entry( const position_fix& pos,
                    entry::Level        level,
                    const string&       msg,
                    const string&       domain )
    : _pos    ( pos    )
    , _level  ( level  )
    , _msg    ( msg    )
    , _domain ( domain )
{}
//=======================================================================================
const position_fix& entry::pos() const
{
    return _pos;
}
//=======================================================================================
string entry::for_std_cxxx() const
{
    vcat res( pos().place(), ' ', level_str() );

    if ( has_domain() )
        res( " {", _domain, '}' );

    res( ":\t", message() );

    return res;
}
//=======================================================================================
const string& entry::message() const
{
    return _msg;
}
//=======================================================================================
entry::Level entry::level() const
{
    return _level;
}
//=======================================================================================
bool entry::is_trace() const
{
    return level() == Level::Trace;
}
//=======================================================================================
bool entry::is_debug() const
{
    return level() == Level::Dbg;
}
//=======================================================================================
bool entry::is_runlog() const
{
    return level() == Level::Runlog;
}
//=======================================================================================
bool entry::is_warning() const
{
    return level() == Level::Warning;
}
//=======================================================================================
bool entry::is_fatal() const
{
    return level() == Level::Fatal;
}
//=======================================================================================
bool entry::has_domain() const
{
    return !_domain.empty();
}
//=======================================================================================
const string& entry::domain() const
{
    return _domain;
}
//=======================================================================================
string entry::level_str() const
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
char entry::level_char() const
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
entry::Level entry::level_from_char( char ch )
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
