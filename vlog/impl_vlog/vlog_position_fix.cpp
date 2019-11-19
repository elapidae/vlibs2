#include "impl_vlog/vlog_position_fix.h"

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
    return vlog::base_name( _file );
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
