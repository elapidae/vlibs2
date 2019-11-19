#include "vlog.h"

#include <algorithm>
#include <vector>
#include <unordered_set>
#include <iostream>
#include <cassert>
#include <memory>

#include "impl_vlog/shared_log.h"
#include "impl_vlog/leveled_log.h"

using namespace impl_vlog;
using namespace std;

//=======================================================================================
//      vlog
//=======================================================================================
static void to_cout( const entry& e );

static std::vector<vlog::Executer>      executers { to_cout };
static std::unordered_set<std::string>  omit_domains {};

static std::unique_ptr<pre_posix::file::shared_log>  shared_log;
static std::unique_ptr<pre_posix::file::leveled_log> leveled_log;
//---------------------------------------------------------------------------------------
static void to_cout( const entry& e )
{
    cout << e.for_std_cxxx();
    cout.flush();
}
//---------------------------------------------------------------------------------------
static void to_cerr( const entry& e )
{
    cerr << e.for_std_cxxx();
    cerr.flush();
}
//---------------------------------------------------------------------------------------
void vlog::clear_executers()
{
    executers.clear();
    shared_log.reset();
    leveled_log.reset();
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
void vlog::_execute( const entry& ent )
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
void vlog::add_shared_log( string fname, uint bytes_in_one, uint rotates )
{
    assert( !shared_log );
    shared_log.reset(new pre_posix::file::shared_log( fname, bytes_in_one, rotates) );

    add_executer( [](const entry& e){ shared_log->write(e); } );
}
//=======================================================================================
void vlog::add_leveled_log( string path, uint bytes_in_one, uint rotates )
{
    assert( !leveled_log );
    leveled_log.reset( new pre_posix::file::leveled_log(path,bytes_in_one,rotates) );

    add_executer( [](const entry& e){ leveled_log->write(e); } );
}
//=======================================================================================
