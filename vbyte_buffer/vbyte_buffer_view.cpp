#include "vbyte_buffer_view.h"

//=======================================================================================
template<typename T>
const char* char_cast( const T* p )
{
    return static_cast<const char*>( static_cast<const void*>(p) );
}
//=======================================================================================

//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view( const char* buf, size_t len )
    : _buffer   ( buf )
    , _remained ( len )
{}
//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view(const signed char *buf, size_t len)
    : _buffer   ( char_cast(buf) )
    , _remained ( len )
{}
//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view(const unsigned char *buf, size_t len)
    : _buffer   ( char_cast(buf) )
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
void vbyte_buffer_view::omit( size_t count )
{
    auto sz = std::min( count, remained() );
    _buffer   += sz;
    _remained -= sz;
}
//=======================================================================================
std::string vbyte_buffer_view::show_string( size_t sz ) const
{
    if ( _remained < sz )
        throw std::out_of_range( "vbyte_buffer_view::show_string: not enouth data" );


    return std::string( _buffer, sz );
}
//=======================================================================================
vbyte_buffer vbyte_buffer_view::show_buffer( size_t sz ) const
{
    return vbyte_buffer{ show_string( sz ) };
}
//=======================================================================================
vbyte_buffer vbyte_buffer_view::show_tail() const
{
    return show_buffer( remained() );
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
vbyte_buffer vbyte_buffer_view::buffer( size_t sz )
{
    return vbyte_buffer{ string(sz) };
}
//=======================================================================================
vbyte_buffer vbyte_buffer_view::tail()
{
    return buffer( remained() );
}
//=======================================================================================
