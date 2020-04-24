#include "vcmdline_parser.h"

#include "vcat.h"
#include <cassert>

using str = std::string;
using cstr = const str&;

//=======================================================================================
//      vcmdline_parser
//=======================================================================================
vcmdline_parser::error::error( cstr msg )
    : std::runtime_error( msg )
{}
//=======================================================================================
vcmdline_parser::vcmdline_parser( int argc, const char * const * const argv )
{
    assert( argc >= 1 && "Too few arguments" );
    //  Copy arguments to own vectors and mark its as unused.
    for ( int i = 0; i < argc; ++i )
    {
        _args.push_back( argv[i] );
        _used.push_back( false   );
    }

    //  Parse application name and path.
    auto pos = _args.at(0).find_last_of( '/' );

    if ( pos == str::npos )
        throw error( vcat("Bad application name: '", _args.at(0),"'.") );

    _app_path = _args.at(0).substr( 0, pos );
    _app_name = _args.at(0).substr( pos + 1 );
}
//=======================================================================================
str vcmdline_parser::app_name() const
{
    return _app_name;
}
//=======================================================================================
str vcmdline_parser::app_path() const
{
    return _app_path;
}
//=======================================================================================
str vcmdline_parser::full_app() const
{
    return _args.at(0);
}
//=======================================================================================
int vcmdline_parser::index( cstr arg ) const
{
    for ( uint i = 1; i < _args.size(); ++i )
    {
        if ( _used.at(i) ) continue;

        if ( _args.at(i) == arg )
            return i;
    }
    return -1;
}
//=======================================================================================
int vcmdline_parser::index_starts_with( cstr preambul ) const
{
    if ( preambul.empty() )
        throw error("Preambul is empty.");

    for ( uint i = 1; i < count(); ++i )
    {
        if ( _used.at(i) ) continue;

        if ( _args.at(i).find(preambul) == 0 )
            return i;
    }
    return -1;
}
//=======================================================================================
uint vcmdline_parser::count() const
{
    return _args.size();
}
//=======================================================================================
cstr vcmdline_parser::get_by_index( int idx ) const
{
    return _args.at( idx );
}
//=======================================================================================
cstr vcmdline_parser::mark_by_index( int idx )
{
    _used.at( idx ) = true;
    return _args.at( idx );
}
//=======================================================================================
std::vector<str> vcmdline_parser::unused() const
{
    std::vector<str> res;
    for ( uint i = 1; i < count(); ++i )
    {
        if ( _used.at(i) ) continue;
        res.push_back( _args.at(i) );
    }
    return res;
}
//=======================================================================================
bool vcmdline_parser::has( cstr arg ) const
{
    return index(arg) > 0;
}
//=======================================================================================
bool vcmdline_parser::has_starts_with( cstr preambul ) const
{
    return index_starts_with( preambul ) > 0;
}
//=======================================================================================
bool vcmdline_parser::take( cstr arg )
{
    auto idx = index( arg );
    if ( idx < 0 ) return false;

    mark_by_index( idx );
    return true;
}
//=======================================================================================
str vcmdline_parser::take_starts_with( cstr preambul )
{
    auto idx = index_starts_with( preambul );
    if ( idx < 0 )
        throw error( vcat("Preambul '",preambul,"' not found.") );

    mark_by_index( idx );
    return _args.at(idx).substr( preambul.size() );
}
//=======================================================================================
str vcmdline_parser::safe_starts_with( cstr preambul, cstr default_val )
{
    auto idx = index_starts_with( preambul );
    if ( idx < 0 ) return default_val;

    mark_by_index( idx );
    return _args.at(idx).substr( preambul.size() );
}
//=======================================================================================
cstr vcmdline_parser::take_next( cstr key )
{
    auto idx = index( key );
    if ( idx < 0 )
        throw error( vcat("Argument '", key, "' not found.") );

    if ( idx == int(count()) || _used.at(idx) )
        throw error( vcat("Cannot take next argument for key'", key, "'.") );

    mark_by_index( idx );
    return mark_by_index( idx + 1 );
}
//=======================================================================================
cstr vcmdline_parser::safe_next( cstr key, cstr default_val )
{
    auto idx = index( key );
    if ( idx < 0 )
        return default_val;

    if ( idx == int(count()) || _used.at(idx) )
        return default_val;

    mark_by_index( idx );
    return mark_by_index( idx + 1 );
}
//=======================================================================================
