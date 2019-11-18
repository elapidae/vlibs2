#ifndef LOG_FILE_H
#define LOG_FILE_H

#include "impl_vlog/pre_posix.h"
#include <atomic>

//=======================================================================================
class pre_posix::file::log_file final
{
public:
    log_file( const std::string& fname );
    ~log_file();

    log_file( log_file&& rhs );
    log_file& operator=( log_file&& rhs );

    void write( const std::string& data );

    void close();

    size_t size() const;

private:
    int _fd = -1;
    std::atomic_size_t _len {0};

    void _move( log_file&& rhs );

    log_file( const log_file& rhs ) = delete;
    log_file& operator=( const log_file& rhs ) = delete;
};
//=======================================================================================

#endif // LOG_FILE_H
