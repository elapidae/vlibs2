#include "shared_log.h"

#include "vcat.h"

//=======================================================================================
pre_posix::file::shared_log::shared_log( std::string path,
                                         uint bytes_in_one,
                                         uint rotates )
    : _log( path, bytes_in_one, rotates )
{
    static std::string start_time = vcat( "------------- BEGIN LOGGING ------------- ",
                                          vtime_point::now().humanable().date_time_zzz(),
                                          " -----------------------\n" );
    _log.write( start_time );
}
//=======================================================================================
pre_posix::file::shared_log::~shared_log()
{
    _log.write( "------------- END LOGGING ------------------------------------"
                "---------------------------\n" );
}
//=======================================================================================
void pre_posix::file::shared_log::write( const impl_vlog::entry& entry )
{
    vcat msg( entry.pos().stamp().humanable().date_time_zzz(), '\t',
              '[', entry.pos().filename(), ':', entry.pos().line(), "]\t",
              entry.level_str(),
              (entry.has_domain() ? vcat("\t{", entry.domain(), '}').str() : "" ), '\t',
              entry.message() );

    _log.write( msg );
}
//=======================================================================================
