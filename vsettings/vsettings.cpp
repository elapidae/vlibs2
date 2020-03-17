#include "vsettings.h"

#include <algorithm>
#include <fstream>
#include "vlog.h"
#include "vbyte_buffer_view.h"
#include "vcat.h"

using namespace std;

//  Type Definitions for debug.
template <class> class TD;


//=======================================================================================
static void add_escaped( string* res, char ch )
{
    static const char hexs[] = "0123456789ABCDEF";

    int i1 = ch >> 4;
    int i2 = ch & 0x0F;

    res->push_back( '\\' );
    res->push_back( 'x' );
    res->push_back( hexs[i1] );
    res->push_back( hexs[i2] );
}
//=======================================================================================


//=======================================================================================
bool vsettings::is_valid_key( cstr key )
{
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
string escape_value( vsettings::cstr val )
{
    string res;
    for ( auto ch: val )
    {
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
        res.push_back( ch );
    }
    return res;
}
//=======================================================================================


//=======================================================================================
struct record
{
    using list = std::vector<record>;

    string key, val;
};
//---------------------------------------------------------------------------------------
struct sub_settings
{
    using list = std::vector<sub_settings>;

    string    name;
    vsettings settings;
};
//---------------------------------------------------------------------------------------
class vsettings::_pimpl
{
public:

    record::list        records;
    sub_settings::list  subs;
};
//=======================================================================================
vsettings::vsettings()
    : _p( std::make_shared<_pimpl>() )
{}
//=======================================================================================
vsettings::~vsettings()
{}
//=======================================================================================
void vsettings::set( cstr key, cstr val )
{
    if ( !is_valid_key(key) )
        throw verror << "Key '" << key << "' is incorrect";

    for ( auto & rec: _p->records )
    {
        if ( rec.key != key ) continue;
        rec.val = val;
        return;
    }

    _p->records.push_back( {key, val} );
}
//=======================================================================================
vsettings::str vsettings::get( cstr key, cstr def_val ) const
{
    for ( auto & rec: _p->records )
    {
        if ( rec.key == key )
            return rec.val;
    }

    return def_val;
}
//=======================================================================================
const vsettings &vsettings::subgroup( cstr name )
{
    if ( !is_valid_subgroup(name) )
        throw verror << "Subgroup name '" << name << "' is incorrect";

    for ( auto & sg: _p->subs )
    {
        if ( sg.name == name )
            return sg.settings;
    }
    _p->subs.push_back( {name,{}} );

    return _p->subs.back().settings;
}
//=======================================================================================
const vsettings &vsettings::subgroup( cstr name ) const
{
    for ( auto & sg: _p->subs )
    {
        if ( sg.name == name )
            return sg.settings;
    }

    throw verror << "Subgroup with name '" << name << "' is absent.";
}
//=======================================================================================
bool vsettings::has( cstr key ) const
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
vsettings::str_list vsettings::keys() const
{
    str_list res;
    for ( auto & rec: _p->records )
        res.push_back( rec.key );

    return res;
}
//=======================================================================================
vsettings::str_list vsettings::subgroup_names() const
{
    str_list res;
    for ( auto & sg: _p->subs )
        res.push_back( sg.name );

    return res;
}
//=======================================================================================
void vsettings::load_from_ini( cstr fname )
{

}
//=======================================================================================
void vsettings::save_to_ini( cstr fname ) const
{
    ofstream f( fname, ios_base::out|ios_base::trunc|ios_base::binary );
    if ( !f.good() )
        throw verror << "Cannot open file '" << fname << "' for save ini.";

    f << save();
}
//=======================================================================================
static void save_keys( vcat* res, string prefix, const vsettings& sett )
{
    auto keys = sett.keys();

    for ( auto key: keys )
    {
        *res << prefix << key << " = " << escape_value(sett.get(key)) << '\n';
    }
}
//---------------------------------------------------------------------------------------
static void save_with_subs( vcat* res, string prefix, const vsettings& sett )
{
    save_keys( res, prefix, sett );

    auto subs = sett.subgroup_names();
    for ( auto sname: subs )
    {
        auto sub_prefix = prefix + sname + '/';
        save_with_subs( res, sub_prefix, sett.subgroup(sname) );
        *res << "\n";
    }
}
//---------------------------------------------------------------------------------------
vsettings::str vsettings::save() const
{
    vcat res("##\n#\n\n");

    save_keys( &res, "", *this );

    res << "\n";
    auto subs = subgroup_names();
    for ( auto sname: subs )
    {
        res << '[' << sname << "]\n";
        save_with_subs( &res, "", subgroup(sname) );
    }

    return res;
}
//=======================================================================================
//static void load_group( vsettings* res, )

void vsettings::load( cstr data )
{
    auto lines = vbyte_buffer::split( data, '\n' );

    auto cur_settings = this;
    for ( auto& line: lines )
    {
        line.trim_spaces();

        str l = line.str();
        if (l.empty()) continue;

        // It is the comment.
        if ( l.at(0) == '#' )
            continue;

        //  It is the 0 group name.
        if ( l.at(0) == '[' )
        {
            cur_settings = this;

        }

    } // for each line
}
//=======================================================================================
