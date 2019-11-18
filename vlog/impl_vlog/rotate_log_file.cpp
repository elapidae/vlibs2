#include "rotate_log_file.h"

#include "vcat.h"

//=======================================================================================
pre_posix::file::rotate_log_file::rotate_log_file( std::string path,
                                                   uint bytes_in_one,
                                                   uint rotates )
    : _path( path )
    , _bytes_in_one( bytes_in_one )
    , _rotates( rotates )
    , _cur_file( path )
{
    if ( _rotates == 0 ) _rotates = 1;
}
//=======================================================================================
void pre_posix::file::rotate_log_file::write( const std::string& data )
{
    std::lock_guard<std::mutex> lock( _write_mutex );

    _cur_file.write( data );

    if ( _cur_file.size() >= _bytes_in_one )
        _rotate_and_reopen_file();
}
//=======================================================================================
// - закрыть базовый файл;
// - найти вакантную позицию;
// - переименовать файлы, заняв в.п.;
// - переименовать только что пользованный файл;
// - открыть базовый файл;
void pre_posix::file::rotate_log_file::_rotate_and_reopen_file()
{
    _cur_file.close();

    uint vacant_pos = 1;
    while ( vacant_pos < _rotates )
    {
        vcat vacant_fname( _path, '.', vacant_pos );
        if ( !pre_posix::file::exists(vacant_fname) )
            break;
        ++vacant_pos;
    }

    for ( auto i = vacant_pos - 1; i >= 1; --i )
    {
        vcat dst_name( _path, '.', i + 1 );
        vcat src_name( _path, '.', i );
        pre_posix::file::rename_safe( src_name, dst_name );
    }

    auto first_dst = _path + ".1";
    pre_posix::file::rename_safe( _path, first_dst );

    _cur_file = pre_posix::file::log_file(_path);
}
//=======================================================================================
