#include "vsettings.h"

#include <algorithm>
#include <fstream>
#include "vlog.h"
#include "vbyte_buffer_view.h"
#include "vcat.h"

using namespace std;

using uint = unsigned int;

//=======================================================================================
static void add_escaped( string* res, char ch )
{
    static const char hexs[] = "0123456789ABCDEF";

    uint8_t i1 = uint8_t(ch) >> 4;
    uint8_t i2 = uint8_t(ch) & 0x0F;

    res->push_back( '\\' );
    res->push_back( 'x' );
    res->push_back( hexs[i1] );
    res->push_back( hexs[i2] );
}
//=======================================================================================


//=======================================================================================
bool vsettings::is_valid_key( cstr key )
{
    if ( key.empty() ) return false;

    for ( char ch: key )
    {
        if ( std::iscntrl(ch) ) return false;
        if ( std::isspace(ch) ) return false;
        if ( std::isblank(ch) ) return false;
        if (!std::isprint(ch) ) return false;
        if ( ch == '\\' ) return false;
        if ( ch == '=' )  return false;
        if ( ch == '/' )  return false;
        if ( ch == '[' )  return false;
        if ( ch == ']' )  return false;
    }
    return true;
}
//=======================================================================================
bool vsettings::is_valid_subgroup( cstr name )
{
    return is_valid_key( name );
}
//=======================================================================================
//  [1] --Parsing UTF-8 characters
//  https://ru.wikipedia.org/wiki/UTF-8
//  if bits == 11 0xx xxx   -- it is two  bytes UTF-8 symbol.
//  if bits == 11 10x xxx   -- it is tree bytes UTF-8 symbol.
//  if bits == 11 110 xxx   -- it is four bytes UTF-8 symbol.
//
//  if first bits == 10 -- it is second and next utf8 symbols.
//
//  0300 == 11000000 mask for testing second and next utf8 symbols.
//
//  Returns: one correct UTF-8 symbol, extracted from begin of sequence
//  If begin of sequence is not correct UTF-8 symbol, returns empty string.
static std::string extract_utf8_symbol( const std::string& sequence )
{
    std::string res;
    if ( sequence.empty() ) return res;

    auto first_ch = sequence.at(0);
    auto is_first_utf8 = (first_ch & 0300) == 0300;   // begins with 11.
    if ( !is_first_utf8 ) return res;

    uint stays = (first_ch & 0040) == 0000 ? 1 :  // two  bytes, stays 1 byte,
                 (first_ch & 0060) == 0040 ? 2 :  // tree bytes, stays 2 bytes,
                 (first_ch & 0070) == 0060 ? 3 :  // four bytes, stays 3 bytes,
                 0;                               // not UTF-8.

    //  Not UTF-8 marker.
    if (stays == 0) return res;

    //  sequence is too short.
    if ( sequence.size() < stays + 1 ) return res;

    //  Check all next symbols, that they begin with 10 bits.
    for ( uint i = 1; i <= stays; ++i )
        if ( (sequence[i] & 0300) != 0200 ) return res;

    //  Form result.
    res.push_back( first_ch );
    for ( uint i = 1; i <= stays; ++i )
        res.push_back( sequence[i] );

    return res;
}
//---------------------------------------------------------------------------------------
string escape_value( vsettings::cstr val )
{
    string res;
    for ( uint i = 0; i < val.size(); ++i )
    {
        auto ch = val.at(i);

        auto is_first_utf8 = (ch & 0300) == 0300;   // begins with 11.
        if ( is_first_utf8 )
        {
            auto utf8 = extract_utf8_symbol( val.substr(i) );
            if ( !utf8.empty() )
            {
                res += utf8;
                i += utf8.size() - 1;
                continue;
            }
        }

        if ( std::iscntrl(ch) )
        {
            add_escaped( &res, ch );
            continue;
        }
        if ( ch == '\n' )
        {
            add_escaped( &res, ch );
            continue;
        }
        if ( ch == '\\' )
        {
            res.push_back( '\\' );
            res.push_back( '\\' );
            continue;
        }
        if ( !isprint(ch) )
        {
            add_escaped( &res, ch );
            continue;
        }
        res.push_back( ch );
    }
    return res;
}
//=======================================================================================
static char from_ascii( char ch, int line_num )
{
    if ( !std::isxdigit(ch) )
        throw verror << "Bad escape sequence in line " << line_num;

    ch = char( std::toupper(ch) );

    if ( ch >= 'A' && ch <= 'F' ) return ch - 'A' + 10;
    if ( ch >= '0' && ch <= '9' ) return ch - '0';

    throw verror << "Bad hex symbol in line " << line_num;
}
//---------------------------------------------------------------------------------------
string unescape_value( const string& val, int line_num )
{
    string res;

    for ( uint i = 0; i < val.size(); ++i )
    {
        char ch = val.at(i);

        if ( ch != '\\' )
        {
            res.push_back( ch );
            continue;
        }

        if ( ++i >= val.size() )
            throw verror << "Escape symbol at end of string in line " << line_num;

        ch = val.at( i );
        if ( ch == '\\' )
        {
            res.push_back( '\\' );
            continue;
        }

        if ( ch != 'x' )
            throw verror << "Cannot determine escape sequence in line " << line_num;

        if ( i + 2 >= val.size() )
            throw verror << "Bad escape sequence in line " << line_num;

        auto hi = val.at( ++i );
        auto lo = val.at( ++i );

        if ( !std::isxdigit(hi) || !std::isxdigit(lo) )
            throw verror << "Bad escape sequence in line " << line_num;

        hi = from_ascii( hi, line_num );
        lo = from_ascii( lo, line_num );

        res.push_back( char((hi << 4)|(lo)) );
    } // for each char

    return res;
}
//=======================================================================================


//=======================================================================================
struct record
{
    using list = std::vector<record>;

    string key, val, comment;
};
//---------------------------------------------------------------------------------------
struct sub_settings
{
    using list = std::vector<sub_settings>;

    string    name, comment;
    vsettings settings;
};
//---------------------------------------------------------------------------------------
class vsettings::_pimpl
{
public:
    //-----------------------------------------------------------------------------------
    enum let_do { let_add, let_error };

    vsettings& subgroup( cstr name, cstr comment, let_do let )
    {
        if ( !is_valid_subgroup(name) )
            throw verror << "Subgroup name '" << name << "' is incorrect";

        for ( auto & sg: subs )
        {
            if ( sg.name == name )
            {
                if ( !comment.empty() )
                    sg.comment = comment;

                return sg.settings;
            }
        }

        if ( let == let_error )
            throw verror << "Subgroup with name '" << name << "' is absent.";

        subs.push_back( {name,comment,{}} );
        return subs.back().settings;
    }
    //-----------------------------------------------------------------------------------
    record::list        records;
    sub_settings::list  subs;
    //-----------------------------------------------------------------------------------
};
//=======================================================================================
vsettings::vsettings()
    : _p( std::make_shared<_pimpl>() )
{}
//=======================================================================================
vsettings::~vsettings()
{}
//=======================================================================================
vsettings::cstr vsettings::comment_of_key( cstr key ) const
{
    for ( auto & rec: _p->records )
    {
        if ( rec.key == key )
            return rec.comment;
    }
    throw verror << "Cannot find key '" << key << "'.";
}
//=======================================================================================
vsettings::cstr vsettings::comment_of_subgroup( cstr name ) const
{
    for ( auto & sub: _p->subs )
    {
        if ( sub.name == name )
            return sub.comment;
    }
    throw verror << "Cannot find subgrop '" << name << "'.";
}
//=======================================================================================
void vsettings::set( cstr key, cstr val, cstr comment )
{
    if ( !is_valid_key(key) )
        throw verror << "Key '" << key << "' is incorrect";

    for ( auto & rec: _p->records )
    {
        if ( rec.key != key ) continue;

        rec.val = val;
        if ( !comment.empty() )
            rec.comment = comment;

        return;
    }

    _p->records.push_back( {key, val, comment} );
}
//=======================================================================================
vsettings::str vsettings::safe_get( cstr key, cstr default_val ) const
{
    if ( !has_key(key) )
        return default_val;

    return get( key );
}
//=======================================================================================
string vsettings::get( cstr key ) const
{
    for ( auto & rec: _p->records )
    {
        if ( rec.key == key )
            return rec.val;
    }
    throw verror << "Value with key '" << key << "' don't found in settings.";
}
//=======================================================================================
vsettings& vsettings::subgroup( cstr name, cstr comment )
{
    return _p->subgroup( name, comment, _pimpl::let_add );
}
//=======================================================================================
const vsettings &vsettings::subgroup( cstr name ) const
{
    return _p->subgroup( name, {}, _pimpl::let_error );
}
//=======================================================================================
bool vsettings::has_key( cstr key ) const
{
    for ( auto & rec: _p->records )
    {
        if ( rec.key == key )
            return true;
    }

    return false;
}
//=======================================================================================
bool vsettings::has_subgroup( cstr name ) const
{
    for ( auto & sg: _p->subs )
    {
        if ( sg.name == name )
            return true;
    }

    return false;
}
//=======================================================================================
bool vsettings::del_key( cstr key )
{
    for ( auto it = _p->records.begin(); it != _p->records.end(); ++it )
    {
        if ( it->key != key ) continue;

        _p->records.erase( it );
        return true;
    }

    return false;
}
//=======================================================================================
bool vsettings::del_subgroup( cstr name )
{
    for ( auto it = _p->subs.begin(); it != _p->subs.end(); ++it )
    {
        if ( it->name != name ) continue;

        _p->subs.erase( it );
        return true;
    }
    return false;
}
//=======================================================================================
vsettings::str_vector vsettings::keys() const
{
    str_vector res;
    for ( auto & rec: _p->records )
        res.push_back( rec.key );

    return res;
}
//=======================================================================================
vsettings::str_vector vsettings::subgroups() const
{
    str_vector res;
    for ( auto & sg: _p->subs )
        res.push_back( sg.name );

    return res;
}
//=======================================================================================
bool vsettings::from_ini_file( cstr fname )
{
    ifstream f( fname, ios_base::in|ios_base::binary );
    //  2020-04-14 - -Было принято решение, что если файла нету, то ничего страшного.
    //  Пусть возвращает
    //throw verror << "Cannot open file '" << fname << "' for load ini.";
    if ( !f.good() )
        return false;

    f.seekg (0, std::ios::end);
    auto fsize = f.tellg();
    if (fsize < 0)
        verror << "Cannot get size of file '" << fname << "'.";

    f.seekg (0, std::ios::beg);

    size_t sz = size_t(fsize);
    std::vector<char> buffer( sz );

    f.read( buffer.data(), fsize );

    from_ini( str{buffer.data(),sz} );

    return true;
}
//=======================================================================================
void vsettings::to_ini_file( cstr fname ) const
{
    ofstream f( fname, ios_base::out|ios_base::trunc|ios_base::binary );
    if ( !f.good() )
        throw verror << "Cannot open file '" << fname << "' for save ini.";

    f << to_ini();
}
//=======================================================================================
static void save_keys( vcat* res, string prefix, const vsettings& sett )
{
    auto keys = sett.keys();

    for ( auto key: keys )
    {
        auto comment = sett.comment_of_key( key );
        if ( !comment.empty() )
            *res << "# " << escape_value(comment) << "\n"; // People can shut to legs.

        *res << prefix << key << " = " << escape_value(sett.get(key)) << '\n';
    }
}
//---------------------------------------------------------------------------------------
static void save_with_subs( vcat* res, string prefix, const vsettings& sett )
{
    save_keys( res, prefix, sett );

    auto subs = sett.subgroups();
    for ( auto sname: subs )
    {
        auto comment = sett.comment_of_subgroup( sname );
        if ( !comment.empty() )
            *res << "# " << escape_value(comment) << '\n';

        auto sub_prefix = prefix + sname + '/';
        save_with_subs( res, sub_prefix, sett.subgroup(sname) );
        *res << "\n";
    }
}
//---------------------------------------------------------------------------------------
vsettings::str vsettings::to_ini() const
{
    vcat res("## INI for NIIAS, with love\n#\n\n");

    save_keys( &res, "", *this );

    res << "\n";
    auto subs = subgroups();
    for ( auto sname: subs )
    {
        auto comment = comment_of_subgroup( sname );

        res << '\n';
        if ( !comment.empty() )
            res << "# " << escape_value(comment) << '\n';

        res << "[" << sname << "]\n";
        save_with_subs( &res, "", subgroup(sname) );
    }

    return res;
}
//=======================================================================================
void vsettings::from_ini( cstr ini )
{
    int line_num = 0;
    auto lines = vbyte_buffer::split( ini, '\n' );

    auto cur_settings = this;
    for ( auto& buf_line: lines )
    {
        ++line_num;
        buf_line.trim_spaces();

        auto line = buf_line.str();
        if (line.empty()) continue;

        //  It is the comment.
        if ( line.at(0) == '#' || line.at(0) == ';' )
            continue;

        //  It is the 0 group name.
        if ( line.at(0) == '[' )
        {
            auto end_pos = line.find( ']' );
            if ( end_pos == str::npos )
                throw verror << "Group not closed by ']' at line " << line_num;

            auto group_name = line.substr( 1, end_pos - 1 );
            cur_settings = &subgroup( group_name );
            continue;
        }

        //  Must be key = value pair.
        auto eq_pos = line.find( '=' );
        if ( eq_pos == str::npos )
            throw verror << "In line " << line_num << " '=' not found.";

        vbyte_buffer key = line.substr( 0, eq_pos );
        key = vbyte_buffer(key).trim_spaces();

        auto val = line.substr(eq_pos + 1);
        val = vbyte_buffer(val).trim_spaces();
        val = unescape_value( val, line_num );

        //  Find subgroup in deep.
        auto subgroups = key.split( '/' );
        auto sub_sett = cur_settings;
        for ( int i = 0; i < int(subgroups.size()) - 1; ++i )
            sub_sett = &sub_sett->subgroup( subgroups.at(uint(i)) );

        sub_sett->set( subgroups.back(), val );
    } // for each line
}
//=======================================================================================
ostream& operator <<( ostream& os, const vsettings& sett )
{
    os << sett.to_ini();
    return os;
}
//=======================================================================================
//      vsettings
//=======================================================================================


//=======================================================================================
//      vsettings::shema
//=======================================================================================
vsettings::schema::_node_iface::~_node_iface()
{}
//=======================================================================================
void vsettings::schema::capture( const vsettings& settings )
{
    for ( auto& node: _nodes )
        node->load( settings );
}
//=======================================================================================
void vsettings::schema::capture_from_ini( cstr fname )
{
    vsettings s;
    s.from_ini_file( fname );
    capture( s );
}
//=======================================================================================
vsettings vsettings::schema::build() const
{
    vsettings res;

    for ( auto& node: _nodes )
        node->save( &res );

    return res;
}
//=======================================================================================
void vsettings::schema::save_to_ini( cstr fname ) const
{
    auto s = build();
    s.to_ini_file( fname );
}
//=======================================================================================
void vsettings::schema::subgroup( cstr name, cstr comment )
{
    _groups.push_back( {name, comment} );
}
//=======================================================================================
void vsettings::schema::end_subgroup()
{
    if ( _groups.empty() )
        throw verror << "Called schema::unsubgroup() with empty groups.";

    _groups.pop_back();
}
//=======================================================================================
void vsettings::schema::_add_node( _node_ptr && new_node )
{
    new_node->groups = _groups;

    for ( auto& node: _nodes )
    {
        if ( node->stored_ptr() == new_node->stored_ptr() )
        {
            throw verror << "Pointer for key '" << new_node->key
                         << "' is same as in key '" << node->key << "'.";
        }

        if ( node->key    == new_node->key &&
             node->groups == new_node->groups )
        {
            throw verror << "Key '" << new_node->key << "' already in the same group.";
        }
    } // Checking the same key and pointer for all nodes.

    _nodes.push_back( std::move(new_node) );
}
//=======================================================================================
bool operator ==( const vsettings::schema::_group_t::vector &lhs,
                  const vsettings::schema::_group_t::vector &rhs )
{
    if ( lhs.size() != rhs.size() ) return false;

    for ( uint i = 0; i < lhs.size(); ++i )
    {
        if ( lhs.at(i).name != rhs.at(i).name ) return false;
    }
    return true;
}
//=======================================================================================
//      vsettings::shema
//=======================================================================================
