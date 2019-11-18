#ifndef LEVELED_LOG_H
#define LEVELED_LOG_H

#include "impl_vlog/rotate_log_file.h"
#include "vlog.h"

//=======================================================================================
class pre_posix::file::leveled_log
{
public:
    leveled_log( std::string path,
                 uint bytes_in_one,
                 uint rotates );

     ~leveled_log();

     void write( const vlog::entry& entry );

 private:
     pre_posix::file::rotate_log_file _t, _d, _r, _w, _f;
};
//=======================================================================================

#endif // LEVELED_LOG_H
