#include "wrap_sys_stat.h"

#include <sys/stat.h>
#include <algorithm>
#include "impl_vposix/linux_call.h"

using namespace impl_vposix;

//=======================================================================================
/*  [1]
 *  Для проверки наличия файла используется вызов stat.
 *  Обещают, что так быстрее, надежней, модней, молодежней.
 *
 *  Ссылка откуда решение было позаимствовано:
 *  https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-usi
ng-standard-c-c11-c
 *
 *   NB! функция exists не совсем exists показывает, может случиться что нет доступа.
*/
//=======================================================================================
/*  [2]
 *  права 0755 -- нормальные стандартные права.
 *  https://help.ubuntu.ru/wiki/стандартные_права_unix
*/
//=======================================================================================



//=======================================================================================
off_t wrap_sys_stat::file_size( const std::string& fname )
{
    struct stat buffer;
    linux_call::check( ::stat, fname.c_str(), &buffer );
    return buffer.st_size;
}
//=======================================================================================
//  см. [1]
bool wrap_sys_stat::file_exists(const std::string &fname)
{
    struct stat buffer;
    return 0 == linux_call::no_err( ::stat, fname.c_str(), &buffer );
}
//=======================================================================================
//  Банально ищем все '/' и пытаемся создать такой каталог.
void wrap_sys_stat::make_path( const std::string& path )
{
    //  Здесь нет опечатки, в заголовке цикла нет условия выхода, оно проверяется в конце
    //  цикла, чтобы пройтись и по итератору path.end().
    for( auto it = path.begin(); ; ++it )
    {
        it = std::find( it, path.end(), '/' );

        std::string cur_dir{ path.begin(), it };
        if ( cur_dir.empty() ) continue; // проверка на корневой каталог.

        mode_t mode = 0755; // [2]
        linux_call::no_err( ::mkdir, cur_dir.c_str(), mode );

        if ( it == path.end() ) break; // для обработки создания самого пути.
    }
}
//=======================================================================================
