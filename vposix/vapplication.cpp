#include "vapplication.h"

#include <cassert>
#include <mutex>

#include "vlog.h"
#include "impl_vposix/poll_context.h"

using namespace impl_vposix;

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

//=======================================================================================
//      args_parser
//=======================================================================================
vapplication::args_parser::args_parser( int argc, const char * const * const argv )
{
    for ( int i = 0; i < argc; ++i )
        _args.push_back( argv[i] );
}
//=======================================================================================
vapplication::args_parser::str vapplication::args_parser::app_name() const
{
    auto pos = _args.at(0).find_last_of( '/' );

    if ( pos == str::npos )
        throw verror << "Bad application name: '" << _args.at(0) << "'.";

    return _args.at(0).substr( pos + 1 );
}
//=======================================================================================
vapplication::args_parser::str vapplication::args_parser::app_path() const
{
    auto pos = _args.at(0).find_last_of( '/' );

    if ( pos == str::npos )
        throw verror << "Bad application name: '" << _args.at(0) << "'.";

    return _args.at(0).substr( 0, pos );
}
//=======================================================================================
vapplication::args_parser::str vapplication::args_parser::full_app() const
{
    return _args.at(0);
}
//=======================================================================================
bool vapplication::args_parser::has( cstr val ) const
{
    for ( auto& a: _args )
    {
        if ( a == val )
            return true;
    }

    return false;
}
//=======================================================================================
vapplication::args_parser::str vapplication::args_parser::get_std( cstr key ) const
{
    for ( auto it = _args.begin(); it != _args.end(); ++it )
    {
        if ( *it != key ) continue;
        auto it_val = it + 1;
        if ( it_val == _args.end() )
            throw verror << "argument key '" << key
                         << "' is last argument, cannot get next argument.";
        return *it_val;
    }
    throw verror << "Cannot fing argument key '" << key << "'.";
}
//=======================================================================================
