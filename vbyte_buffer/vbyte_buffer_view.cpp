#include "vbyte_buffer_view.h"

//=======================================================================================
template<typename Res, typename T>
const Res* any_cast( const T* p )
{
    return static_cast<const Res*>( static_cast<const void*>(p) );
}
//=======================================================================================

//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view( const char* buf, size_t len )
    : _buffer   ( buf )
    , _remained ( len )
{}
//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view( const signed char *buf, size_t len )
    : _buffer   ( any_cast<char>(buf) )
    , _remained ( len )
{}
//=======================================================================================
vbyte_buffer_view::vbyte_buffer_view( const unsigned char *buf, size_t len )
    : _buffer   ( any_cast<char>(buf) )
    , _remained ( len )
{}
//=======================================================================================
const char *vbyte_buffer_view::data() const
{
    return _buffer;
}
//=======================================================================================
const int8_t *vbyte_buffer_view::sdata() const
{
    return any_cast<int8_t>( _buffer );
}
//=======================================================================================
const uint8_t *vbyte_buffer_view::udata() const
{
    return any_cast<uint8_t>( _buffer );
}
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
        throw std::out_of_range( "vbyte_buffer_view::show_string: not enough data" );


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
