#include "vlog.h"

#include <vector>
#include <unordered_set>
#include <iostream>
#include <cassert>
#include <memory>

#include "vcat.h"

//=======================================================================================
//  UPD 2020-11-04 -- omit file loggers in windows systems.
#ifndef V_LOG_NOT_USE_POSIX

    #include "impl_vlog/shared_log.h"
    #include "impl_vlog/leveled_log.h"

    static std::unique_ptr<pre_posix::file::shared_log>  shared_log;
    static std::unique_ptr<pre_posix::file::leveled_log> leveled_log;

    static void clear_file_loggers()
    {
        shared_log.reset();
        leveled_log.reset();
    }

#else

    static void clear_file_loggers()
    {}

#endif
//=======================================================================================


using namespace impl_vlog;
using namespace std;

//=======================================================================================
//      vlog
//=======================================================================================
static void to_cout( const entry& e );

static std::vector<vlog::Executer>      executers { to_cout };
static std::unordered_set<std::string>  omit_domains {};

//---------------------------------------------------------------------------------------
static string for_std_cxxx( const entry& e )
{
    vcat res( e.pos().place(), ' ', e.level_str() );

    if ( e.has_domain() )
        res( " {", e.domain(), '}' );

    res( ":\t", e.message() );

    return res;
}
//---------------------------------------------------------------------------------------
static void to_cout( const entry& e )
{
    cout << for_std_cxxx(e);
    cout.flush();
}
//---------------------------------------------------------------------------------------
static void to_cerr( const entry& e )
{
    cerr << for_std_cxxx(e);
    cerr.flush();
}
//---------------------------------------------------------------------------------------
void vlog::clear_executers()
{
    executers.clear();
    clear_file_loggers();
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
void vlog::apply_domain( const string& domain )
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
bool vlog::_need_omit_domain( const string& domain )
{
    return omit_domains.count( domain );
}
//=======================================================================================
#ifndef V_LOG_NOT_USE_POSIX
void vlog::set_shared_log( const string& fname, unsigned filesize, unsigned rotates )
{
    assert( !shared_log );
    shared_log.reset(new pre_posix::file::shared_log( fname, filesize, rotates) );

    add_executer( [](const entry& e){ shared_log->write(e); } );
}

void vlog::set_leveled_log( const string& path, unsigned filesize, unsigned rotates )
{
    assert( !leveled_log );
    leveled_log.reset( new pre_posix::file::leveled_log(path,filesize,rotates) );

    add_executer( [](const entry& e){ leveled_log->write(e); } );
}
#else
//=======================================================================================
//  Not deprecated, but do not work!
[[deprecated]]
void vlog::set_shared_log( const string&, unsigned, unsigned )
{}

//  Not deprecated, but do not work!
[[deprecated]]
void vlog::set_leveled_log( const string&, unsigned, unsigned )
{}
#endif
//=======================================================================================
