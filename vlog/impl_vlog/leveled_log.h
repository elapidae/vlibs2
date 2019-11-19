#ifndef IMPL_VLOG_LEVELED_LOG_H
#define IMPL_VLOG_LEVELED_LOG_H

#include "impl_vlog/rotate_log_file.h"
#include "impl_vlog/entry.h"

//=======================================================================================
class pre_posix::file::leveled_log final
{
public:
    leveled_log( std::string path,
                 uint bytes_in_one,
                 uint rotates );

     ~leveled_log();

     void write( const impl_vlog::entry& entry );

 private:
     pre_posix::file::rotate_log_file _t, _d, _r, _w, _f;
};
//=======================================================================================

#endif // IMPL_VLOG_LEVELED_LOG_H
