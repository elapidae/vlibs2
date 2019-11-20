#include "impl_vlog/logger.h"

#include <cassert>
#include "vlog.h"

using namespace impl_vlog;
using namespace std;

//=======================================================================================
//      logger
//=======================================================================================
logger::logger( position_fix && pos,
                entry::Level    level )
    : _pos   ( pos   )
    , _level ( level )
{
    // Включаем флаги по умолчанию.
    _stream << std::showbase << std::boolalpha;
    space();
}
//=======================================================================================
logger::~logger()
{
    if ( !_is_on ) return;

    auto msg = _stream.str();
    if ( _delimiter_has_been_added() )
        msg.back() = '\n';
    else
        msg.push_back( '\n' );

    auto ent = entry( _pos, _level, msg, _domain );
    vlog::_execute( ent );
}
//=======================================================================================
logger& logger::operator[]( bool on )
{
    _is_on = on;
    return *this;
}
//=======================================================================================
logger& logger::operator[]( const string& domain )
{
    assert( _domain.empty() );
    _domain = domain;
    return *this;
}
//=======================================================================================
logger& logger::operator[]( const char* domain )
{
    assert( _domain.empty() );
    _domain = domain;
    return *this;
}
//=======================================================================================
//      logger
//=======================================================================================
