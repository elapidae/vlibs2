#ifndef SHARED_LOG_H
#define SHARED_LOG_H

#include "impl_vlog/rotate_log_file.h"
#include "vlog.h"

//=======================================================================================
class pre_posix::file::shared_log final
{
public:
    shared_log( std::string path,
                uint bytes_in_one,
                uint rotates );

    ~shared_log();

    void write( const vlog::entry& entry );

private:
    pre_posix::file::rotate_log_file _log;
};
//=======================================================================================

#endif // SHARED_LOG_H
