#include "log_file.h"


//=======================================================================================
pre_posix::file::log_file::log_file( const std::string& fname )
    : _fd  ( file::open_write_append(fname) )
    , _len ( file::size(fname) )
{}
//=======================================================================================
pre_posix::file::log_file::~log_file()
{
    close();
}
//=======================================================================================
void pre_posix::file::log_file::_move( pre_posix::file::log_file &&rhs )
{
    std::swap( _fd,  rhs._fd  );
    std::swap( _len, rhs._len );
}
//=======================================================================================
pre_posix::file::log_file::log_file( pre_posix::file::log_file&& rhs )
{
    _move( std::move(rhs) );
}
//=======================================================================================
pre_posix::file::log_file&
pre_posix::file::log_file::operator=( pre_posix::file::log_file&& rhs )
{
    if (this == &rhs) return *this;
    _move( std::move(rhs) );
    return *this;
}
//=======================================================================================
void pre_posix::file::log_file::write( const std::string& data )
{
    file::write( _fd, data );
    _len += data.size();
}
//=======================================================================================
void pre_posix::file::log_file::close()
{
    //  if fd < 0, nothing strange happen.
    file::close( _fd );
    _fd = -1;
}
//=======================================================================================
off_t pre_posix::file::log_file::size() const
{
    return _len;
}
//=======================================================================================
