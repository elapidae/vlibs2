#ifndef ROTATE_LOG_FILE_H
#define ROTATE_LOG_FILE_H

#include <mutex>
#include "impl_vlog/log_file.h"

//=======================================================================================
class pre_posix::file::rotate_log_file
{
public:
    //  NB! Если rotates == 0, то автоматически выставляется в единицу, т.к. иначе
    //  базовый файл будет перетираться и записываться с нуля, а это чушь какая-то.
    rotate_log_file( std::string path,
                     uint bytes_in_one,
                     uint rotates );

    void write( const std::string& data );

private:
    std::mutex _write_mutex;

    std::string _path;
    uint _bytes_in_one;
    uint _rotates;

    pre_posix::file::log_file _cur_file;

    void _rotate_and_reopen_file();
};
//=======================================================================================

#endif // ROTATE_LOG_FILE_H
