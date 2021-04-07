#include "impl_vlog/position_fix.h"

#include <algorithm>
#include "vcat.h"
#include "vlog.h"

using namespace impl_vlog;

//=======================================================================================
position_fix::position_fix( const char* file,
                            int         line,
                            const char* func )
    : _stamp ( vtime_point::now() )
    , _file  ( file )
    , _line  ( line )
    , _func  ( func )
{}
//=======================================================================================
//=======================================================================================
std::string position_fix::place() const
{
    return vcat( '[', filename(), ':', line(), ']' );
}
//=======================================================================================
//=======================================================================================
const vtime_point& position_fix::stamp() const
{
    return _stamp;
}
//=======================================================================================
const char* position_fix::filepath() const
{
    return _file;
}
//=======================================================================================
std::string position_fix::filename() const
{
    std::string fpath( _file );
    auto slash_it = std::find( fpath.rbegin(), fpath.rend(), '/' );

    //  Correction, if path use window slashes.
    if ( slash_it == fpath.rend() )
        slash_it = std::find( fpath.rbegin(), fpath.rend(), '\\' );

    return { slash_it.base(), fpath.end() };
}
//=======================================================================================
int32_t position_fix::line() const
{
    return _line;
}
//=======================================================================================
const char* position_fix::function() const
{
    return _func;
}
//=======================================================================================
