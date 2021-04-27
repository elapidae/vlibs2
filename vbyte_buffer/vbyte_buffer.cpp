#include "vbyte_buffer.h"

#include <algorithm>
#include "vbyte_buffer_view.h"

using namespace std;

//=======================================================================================
//      General & simple parts: ctos, str(), size(), etc.
//=======================================================================================
vbyte_buffer::vbyte_buffer()
{}
//=======================================================================================
vbyte_buffer::vbyte_buffer( std::string seed )
    : _buf( std::move(seed) )
{}
//=======================================================================================
vbyte_buffer::vbyte_buffer( const char* seed )
    : _buf( seed )
{}
//=======================================================================================
const std::string& vbyte_buffer::str() const noexcept
{
    return _buf;
}
//=======================================================================================
size_t vbyte_buffer::size() const noexcept
{
    return _buf.size();
}
//=======================================================================================
bool vbyte_buffer::empty() const noexcept
{
    return _buf.empty();
}
//=======================================================================================
const char *vbyte_buffer::data() const noexcept
{
    return _buf.data();
}
//=======================================================================================
const int8_t *vbyte_buffer::sdata() const noexcept
{
    return static_cast<const int8_t*>( static_cast<const void*>(data()) );
}
//=======================================================================================
const uint8_t *vbyte_buffer::udata() const noexcept
{
    return static_cast<const uint8_t*>( static_cast<const void*>(data()) );
}
//=======================================================================================
vbyte_buffer_view vbyte_buffer::view() const
{
    return { _buf.data(), _buf.size() };
}
//=======================================================================================
void vbyte_buffer::clear()
{
    _buf.clear();
}
//=======================================================================================
bool vbyte_buffer::operator ==( const vbyte_buffer& rhs ) const noexcept
{
    return _buf == rhs._buf;
}
//=======================================================================================
bool vbyte_buffer::operator !=( const vbyte_buffer& rhs ) const noexcept
{
    return _buf != rhs._buf;
}
//=======================================================================================
vbyte_buffer& vbyte_buffer::operator += ( const vbyte_buffer& rhs )
{
    _buf += rhs._buf;
    return *this;
}
//=======================================================================================
vbyte_buffer::operator const std::string&() const noexcept
{
    return _buf;
}
//=======================================================================================
//      appends
//=======================================================================================
void vbyte_buffer::append( unsigned char val )
{
    _buf.push_back( char(val) );
}
//=======================================================================================
void vbyte_buffer::append( signed char val )
{
    _buf.push_back( char(val) );
}
//=======================================================================================
void vbyte_buffer::append( char val )
{
    _buf.push_back( val );
}
//=======================================================================================
void vbyte_buffer::append( const std::string& val )
{
    _buf.append( val );
}
//=======================================================================================
void vbyte_buffer::append( const char * val )
{
    _buf.append( val );
}
//=======================================================================================
void vbyte_buffer::append( bool b )
{
    signed char ch = b ? -1 : 0;
    append( ch );
}
//=======================================================================================
//      chop, resize, left, right, middle
//=======================================================================================
void vbyte_buffer::chop_front( size_t count )
{
    _buf.erase( 0, count );
}
//=======================================================================================
void vbyte_buffer::chop_back( size_t count )
{
    auto new_size = _buf.size() > count
                        ? _buf.size() - count
                        : 0;

    _buf.resize( new_size );
}
//=======================================================================================
void vbyte_buffer::resize( size_t count )
{
    if ( _buf.size() < count )
        throw std::length_error( string("vbyte_buffer::resize(" +
                                 to_string(count) + "), but size = " +
                                 to_string(_buf.size())) );

    _buf.resize( count );
}
//=======================================================================================
vbyte_buffer vbyte_buffer::left( size_t count ) const
{
    return vbyte_buffer{ _buf.substr(0,count) };
}
//=======================================================================================
vbyte_buffer vbyte_buffer::right( size_t count ) const
{
    if ( _buf.size() < count )
        return vbyte_buffer{ _buf };

    return vbyte_buffer{ _buf.substr(_buf.size() - count) };
}
//=======================================================================================
vbyte_buffer vbyte_buffer::middle( size_t pos, size_t len ) const
{
    return vbyte_buffer { _buf.substr(pos, len) };
}
//=======================================================================================
//      starts, ends checking
//=======================================================================================
bool vbyte_buffer::starts_with( const std::string& what ) const
{
    return _buf.find(what) == 0;
}
//=======================================================================================
bool vbyte_buffer::ends_with( const std::string& what ) const
{
    if ( _buf.size() < what.size()  ) return false;

    return _buf.rfind(what) == size() - what.size();
}
//=======================================================================================
//      HEX
//=======================================================================================
static auto constexpr hex_syms = "0123456789abcdef";
static auto constexpr HEX_syms = "0123456789ABCDEF";
//---------------------------------------------------------------------------------------
static int ch_from_hex(char ch)
{
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'F') return 10 + ch - 'A';
    if (ch >= 'a' && ch <= 'f') return 10 + ch - 'a';
    return -1;
}
//---------------------------------------------------------------------------------------
//  Separator wiil ignore if with_separator == false.
static vbyte_buffer _to_hex( const std::string& src,
                             const char* hsyms,
                             bool with_separator,
                             char separator )
{
    std::stringstream ss;

    for ( auto ch: src )
    {
        ss << hsyms[(ch >> 4) & 0xF] << hsyms[ch & 0xF];

        if ( with_separator )
            ss << separator;
    }

    auto res = ss.str();

    if ( with_separator && !src.empty() )   // Delete last space.
        res.pop_back();                     //

    return vbyte_buffer{ std::move(res) };
}
//---------------------------------------------------------------------------------------
vbyte_buffer vbyte_buffer::from_hex( const std::string& src )
{
    std::string res;
    res.resize( (src.size() + 1)/2 );

    auto cur = res.rbegin();
    size_t real_res_size = 0;

    bool in_char = false;
    for ( auto src_it = src.rbegin(); src_it != src.rend(); ++src_it )
    {
        int tmp = ch_from_hex( *src_it );
        if (tmp < 0) continue;

        if ( in_char )
        {
            *cur++ |= tmp << 4;
        }
        else
        {
            *cur = static_cast<char>( tmp );
            ++real_res_size;
        }
        in_char = !in_char;
    }

    return vbyte_buffer { res.erase(0, res.size() - real_res_size) };
}
//=======================================================================================
vbyte_buffer vbyte_buffer::from_hex() const
{
    return from_hex( *this );
}
//=======================================================================================
vbyte_buffer vbyte_buffer::tohex() const
{
    return _to_hex( *this, hex_syms, false, '\0' );
}
//=======================================================================================
vbyte_buffer vbyte_buffer::toHex() const
{
    return _to_hex( *this, HEX_syms, false, '\0' );
}
//=======================================================================================
vbyte_buffer vbyte_buffer::to_hex( char separator ) const
{
    return _to_hex( *this, hex_syms, true, separator );
}
//=======================================================================================
vbyte_buffer vbyte_buffer::to_Hex( char separator ) const
{
    return _to_hex( *this, HEX_syms, true, separator );
}
//=======================================================================================
bool vbyte_buffer::is_hex_symbol( char ch ) noexcept
{
    return ch_from_hex(ch) >= 0;
}
//=======================================================================================
//      split by separators and spaces, trim spaces
//=======================================================================================
vbyte_buffer::vector vbyte_buffer::split( const std::string& data, char splitter )
{
    vector res;

    auto cur = data.begin();
    while ( cur != data.end() )
    {
        auto next = std::find( cur, data.end(), splitter );
        res.push_back( vbyte_buffer{{cur, next}} );

        if ( next == data.end() ) break;
        cur = next + 1;
    }
    return res;
}
//=======================================================================================
vbyte_buffer::vector vbyte_buffer::split( char splitter ) const
{
    return split( *this, splitter );
}
//=======================================================================================
//  Нужен, чтобы в find_if типы совпадали.
static bool is_space( char ch )
{
    return std::isspace( ch );
}
//---------------------------------------------------------------------------------------
vbyte_buffer::vector vbyte_buffer::split_by_spaces() const
{
    vector res;

    auto cur = _buf.begin();
    while ( cur != _buf.end() )
    {
        auto next = std::find_if( cur, _buf.end(), is_space );

        if ( cur != next )                                  //  omit empties
            res.push_back( vbyte_buffer({cur,next}) );      //

        if ( next == _buf.end() ) break;
        cur = next + 1;
    }
    return res;
}
//=======================================================================================
vbyte_buffer& vbyte_buffer::trim_spaces()
{
    if ( _buf.empty() ) return *this;

    auto from = _buf.begin();
    while( from != _buf.end() && std::isspace(*from) )
        ++from;

    auto to = _buf.end() - 1;
    while( to != from && std::isspace(*to) )
        --to;

    _buf = {from, to + 1};

    return *this;
}
//=======================================================================================
//      print
//=======================================================================================
ostream& operator <<( ostream& os, const vbyte_buffer& buf )
{
    os << buf.str();
    return os;
}
//=======================================================================================
vbyte_buffer operator +( const vbyte_buffer& lhs, const vbyte_buffer& rhs )
{
    auto res = lhs;
    res += rhs;
    return res;
}
//=======================================================================================
