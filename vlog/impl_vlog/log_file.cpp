#include "log_file.h"


//=======================================================================================
//  Просто якоримся на файл. Если кто-то будет его потрошить из других потоков все равно
//  не поможем. Поэтому просто береме файлы на дозапись.
pre_posix::file::log_file::log_file( const std::string& fname )
    : _fd  ( file::open_write_append(fname) )
    , _len ( size_t(file::size(fname)) )
{}
//=======================================================================================
pre_posix::file::log_file::~log_file()
{
    //  if fd < 0 -- nothing strange happen.
    file::close( _fd );
}
//=======================================================================================
pre_posix::file::log_file::log_file( pre_posix::file::log_file&& rhs )
{
    std::swap( _fd, rhs._fd );
}
//=======================================================================================
pre_posix::file::log_file&
pre_posix::file::log_file::operator=( pre_posix::file::log_file&& rhs )
{
    if (this == &rhs) return *this;
    std::swap( _fd, rhs._fd );
    return *this;
}
//=======================================================================================
void pre_posix::file::log_file::write( const std::string& data )
{
    file::write( _fd, data );
    _len += data.size();
}
//=======================================================================================
size_t pre_posix::file::log_file::size() const
{
    return _len;
}
//=======================================================================================
