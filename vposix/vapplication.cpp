#include "vapplication.h"

#include <cassert>
#include <mutex>

#include "vlog.h"
#include "vcat.h"
#include "impl_vposix/poll_context.h"
#include "impl_vposix/wrap_unistd.h"

using namespace impl_vposix;

using str  = std::string;
using cstr = const str&;

//=======================================================================================
//      vapplication
//=======================================================================================

//----------------------------------------
//  Can be only as static variable.
static poll_context *first_ctx = poll_context::current();
//----------------------------------------

//=======================================================================================
void vapplication::_invoke( task_type && f )
{
    first_ctx->push( std::move(f) );
}
//=======================================================================================
size_t vapplication::tasks_count() const
{
    return first_ctx->tasks_count();
}
//=======================================================================================
void vapplication::tasks_clear()
{
    first_ctx->tasks_clear();
}
//=======================================================================================
void vapplication::poll()
{
    first_ctx->poll();
}
//=======================================================================================
void vapplication::stop()
{
    first_ctx->stop();
}
//=======================================================================================
pid_t vapplication::pid() noexcept
{
    return wrap_unistd::get_pid();
}
//=======================================================================================

//=======================================================================================
//      args_parser
//=======================================================================================
vapplication::args_parser::error::error( cstr msg )
    : std::runtime_error( msg )
{}
//=======================================================================================
vapplication::args_parser::args_parser( int argc, const char * const * const argv )
{
    for ( int i = 0; i < argc; ++i )
    {
        _args.push_back( argv[i] );
        _used.push_back( false   );
    }
}
//=======================================================================================
str vapplication::args_parser::app_name() const
{
    auto pos = _args.at(0).find_last_of( '/' );

    if ( pos == str::npos )
        throw verror << "Bad application name: '" << _args.at(0) << "'.";

    return _args.at(0).substr( pos + 1 );
}
//=======================================================================================
str vapplication::args_parser::app_path() const
{
    auto pos = _args.at(0).find_last_of( '/' );

    if ( pos == str::npos )
        throw verror << "Bad application name: '" << _args.at(0) << "'.";

    return _args.at(0).substr( 0, pos );
}
//=======================================================================================
str vapplication::args_parser::full_app() const
{
    return _args.at(0);
}
//=======================================================================================
int vapplication::args_parser::index( cstr arg ) const
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
int vapplication::args_parser::index_starts_with( cstr preambul ) const
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
uint vapplication::args_parser::count() const
{
    return _args.size();
}
//=======================================================================================
cstr vapplication::args_parser::get_by_index( int idx ) const
{
    return _args.at( idx );
}
//=======================================================================================
cstr vapplication::args_parser::mark_by_index( int idx )
{
    _used.at( idx ) = true;
    return _args.at( idx );
}
//=======================================================================================
std::vector<str> vapplication::args_parser::unused() const
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
bool vapplication::args_parser::has( cstr arg ) const
{
    return index(arg) > 0;
}
//=======================================================================================
bool vapplication::args_parser::has_starts_with( cstr preambul ) const
{
    return index_starts_with( preambul ) > 0;
}
//=======================================================================================
bool vapplication::args_parser::take( cstr arg )
{
    auto idx = index( arg );
    if ( idx < 0 ) return false;

    mark_by_index( idx );
    return true;
}
//=======================================================================================
str vapplication::args_parser::take_starts_with( cstr preambul )
{
    auto idx = index_starts_with( preambul );
    if ( idx < 0 )
        throw error( vcat("Preambul '",preambul,"' not found.") );

    mark_by_index( idx );
    return _args.at(idx).substr( preambul.size() );
}
//=======================================================================================
str vapplication::args_parser::safe_starts_with( cstr preambul, cstr default_val )
{
    auto idx = index_starts_with( preambul );
    if ( idx < 0 ) return default_val;

    mark_by_index( idx );
    return _args.at(idx).substr( preambul.size() );
}
//=======================================================================================
cstr vapplication::args_parser::take_next( cstr key )
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
cstr vapplication::args_parser::safe_next( cstr key, cstr default_val )
{
    auto idx = index( key );
    if ( idx < 0 )
        return default_val;

    if ( idx == int(count()) || _used.at(idx) )
        return default_val;

    return mark_by_index( idx );
}
//=======================================================================================

