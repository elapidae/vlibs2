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
    _fd = -1;
}
//=======================================================================================
void pre_posix::file::log_file::_move(pre_posix::file::log_file &&rhs)
{
    std::swap( _fd, rhs._fd );

    size_t tmp = _len;
    _len.exchange( rhs._len );
    rhs._len.exchange( tmp );
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
    file::close( _fd );
    _fd = -1;
}
//=======================================================================================
size_t pre_posix::file::log_file::size() const
{
    return _len;
}
//=======================================================================================
