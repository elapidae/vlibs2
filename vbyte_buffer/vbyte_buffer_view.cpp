#include "vbyte_buffer_view.h"

//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view( const char* buf, size_t len )
    : _buffer   ( buf )
    , _remained ( len )
{}
//=======================================================================================
size_t vbyte_buffer_view::remained() const
{
    return _remained;
}
//=======================================================================================
bool vbyte_buffer_view::finished() const
{
    return _remained == 0;
}
//=======================================================================================
std::string vbyte_buffer_view::show_string( size_t sz ) const
{
    if ( _remained < sz )
        throw std::out_of_range( "vbyte_buffer_view::show_string: not enouth data" );


    return std::string( _buffer, sz );
}
//=======================================================================================
std::string vbyte_buffer_view::string( size_t sz )
{
    auto res = show_string( sz );

    _buffer   += sz;
    _remained -= sz;

    return res;
}
//=======================================================================================
