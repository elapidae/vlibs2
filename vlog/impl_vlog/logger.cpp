#include "impl_vlog/logger.h"

#include <cassert>
#include "vlog.h"

using namespace impl_vlog;
using namespace std;

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
logger::~logger() noexcept(false)
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
logger& logger::operator[]( std::string domain )
{
    assert( _domain.empty() );
    _domain = std::move( domain );

    if ( vlog::_need_omit_domain(_domain) )
        _is_on = false;

    return *this;
}
//=======================================================================================
logger& logger::operator[]( const char* domain )
{
    return operator[]( std::string(domain) );
}
//=======================================================================================
