#include "vlog.h"

#include <algorithm>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <cassert>

using namespace std;

//=======================================================================================
//      vlog
//=======================================================================================
static void to_cout( const vlog::entry& e );

static std::vector<vlog::Executer>      executers { to_cout };
static std::unordered_set<std::string>  omit_domains {};
//---------------------------------------------------------------------------------------
static void to_cout( const vlog::entry& e )
{
    cout << e.for_std_cxxx();
    cout.flush();
}
//---------------------------------------------------------------------------------------
static void to_cerr( const vlog::entry& e )
{
    cerr << e.for_std_cxxx();
    cerr.flush();
}
//---------------------------------------------------------------------------------------
void vlog::clear_executers()
{
    executers.clear();
}
//---------------------------------------------------------------------------------------
void vlog::add_executer( vlog::Executer e )
{
    executers.push_back( e );
}
//---------------------------------------------------------------------------------------
void vlog::add_log_to_cout()
{
    executers.push_back( to_cout );
}
//---------------------------------------------------------------------------------------
void vlog::add_log_to_cerr()
{
    executers.push_back( to_cerr );
}
//---------------------------------------------------------------------------------------
void vlog::omit_domain( const string& domain )
{
    omit_domains.insert( domain );
}
//---------------------------------------------------------------------------------------
void vlog::apply_domain(const string &domain)
{
    omit_domains.erase( domain );
}
//---------------------------------------------------------------------------------------
void vlog::_execute( const vlog::entry& ent )
{
    if ( omit_domains.count(ent.domain()) ) return;

    for ( auto& exe: executers )
        exe( ent );
}
//=======================================================================================
string vlog::base_name(const char *file)
{
    string fpath( file );
    auto slash_it = std::find( fpath.rbegin(), fpath.rend(), '/' );
    return { slash_it.base(), fpath.end() };
}
//=======================================================================================
//      vlog
//=======================================================================================
