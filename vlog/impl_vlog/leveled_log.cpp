#include "leveled_log.h"

#include "vcat.h"

using namespace impl_vlog;

//=======================================================================================
static const std::string& mk_path( const std::string& path )
{
    pre_posix::file::make_path( path );
    return path;
}
//---------------------------------------------------------------------------------------
pre_posix::file::leveled_log::leveled_log( std::string path,
                                           uint bytes_in_one,
                                           uint rotates )

    : _t( mk_path(path) + "/trace.log",   bytes_in_one, rotates )
    , _d(         path  + "/debug.log",   bytes_in_one, rotates )
    , _r(         path  + "/runlog.log",  bytes_in_one, rotates )
    , _w(         path  + "/warning.log", bytes_in_one, rotates )
    , _f(         path  + "/fatal.log",   bytes_in_one, rotates )
{
    _t.write( pre_posix::file::start_line_for_log() );
    _d.write( pre_posix::file::start_line_for_log() );
    _r.write( pre_posix::file::start_line_for_log() );
    _w.write( pre_posix::file::start_line_for_log() );
    _f.write( pre_posix::file::start_line_for_log() );
}
//=======================================================================================
pre_posix::file::leveled_log::~leveled_log()
{
    _t.write( pre_posix::file::fin_line_for_log() );
    _d.write( pre_posix::file::fin_line_for_log() );
    _r.write( pre_posix::file::fin_line_for_log() );
    _w.write( pre_posix::file::fin_line_for_log() );
    _f.write( pre_posix::file::fin_line_for_log() );
}
//=======================================================================================
void pre_posix::file::leveled_log::write( const impl_vlog::entry& entry )
{
    vcat msg( entry.pos().stamp().humanable().date_time_zzz(), '\t',
              (entry.has_domain() ? vcat("\t{", entry.domain(), '}').str() : "" ), '\t',
              entry.message() );

    switch (entry.level())
    {
    case entry::Level::Trace:     _t.write( msg ); break;
    case entry::Level::Dbg:       _d.write( msg ); break;
    case entry::Level::Runlog:    _r.write( msg ); break;
    case entry::Level::Warning:   _w.write( msg ); break;
    case entry::Level::Fatal:     _f.write( msg ); break;
    }
}
//=======================================================================================
