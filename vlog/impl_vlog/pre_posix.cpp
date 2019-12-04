#include "pre_posix.h"

#include <string.h> // OMG!! for strerror_r()

/****************************************************************************************
 *  [1] sys/stat.h
 *
 *  Для проверки наличия файла используется функция stat( fname, struct stat ).
 *  Обещают, что она быстрее, надежней, модней, молодежней.
 *
 *  Ссылка откуда решение было позаимствовано:
 *  https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-usi
ng-standard-c-c11-c
 *
 *  NB! функция exists не совсем exists показывает, может случиться что нет доступа.
 *
****************************************************************************************/
#include <sys/stat.h>
//=======================================================================================



#include "vcat.h"
#include "vlog.h"

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

//=======================================================================================
string pre_posix::file::start_line_for_log()
{
    return vcat( "------------- BEGIN LOGGING ------------- ",
                 vtime_point::now().humanable().date_time_zzz(),
                 " -----------------------\n" );
}
//=======================================================================================
string pre_posix::file::fin_line_for_log()
{
    return "------------- END LOGGING ------------------------------------"
           "---------------------------\n";
}
//=======================================================================================


//=======================================================================================
string pre_posix::err_msg()
{
    constexpr auto buf_size = 1024;
    char buf[ buf_size ];

    //  TS версия.
    return vcat('[', errno, "] ", ::strerror_r(errno, buf, buf_size) );
}
//=======================================================================================


//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnoexcept-type"
template<typename Fun, typename ... Args>
auto call_no_err( Fun fun, Args ... args) -> decltype( fun(args...) )
{
    decltype( fun(args...) ) res;

    do
    {
        res = fun( args ... );
    }
    while ( res == -1 && errno == EINTR );

    return res;
}
#pragma GCC diagnostic pop
//=======================================================================================
template<typename Fun, typename ... Args>
auto call_check( Fun fun, Args ... args) -> decltype( fun(args...) )
{
    auto res = call_no_err( fun, args... );
    if ( res < 0 )
        throw verror << pre_posix::err_msg();

    return res;
}
//=======================================================================================
template<typename Fun>
auto call_check( Fun fun, const char* fname, struct stat* s) -> decltype( fun(fname,s) )
{
    auto res = call_no_err( fun, fname, s );
    if ( res < 0 ) throw verror << pre_posix::err_msg();

    return res;
}
//=======================================================================================
static
auto call_check_stat( const char* fname, struct stat* s) -> decltype( ::stat(fname,s) )
{
    auto res = call_no_err( ::stat, fname, s );
    if ( res < 0 ) throw verror( "filename: '", fname,
                                 ", err_msg: ", pre_posix::err_msg() );

    return res;
}
//=======================================================================================
static
auto call_check_open( const std::string& fname, int flags, mode_t mode )
                                        -> decltype( ::open(fname.c_str(),flags,mode) )
{
    auto res = call_no_err( ::open, fname.c_str(), flags, mode );
    if ( res < 0 ) throw verror( std::oct,
                                 "filename: '", fname, "'",
                                 ", flags: ", flags,
                                 ", mode: ", mode,
                                 ", err_msg: ", pre_posix::err_msg() );

    return res;
}
//=======================================================================================

//=======================================================================================
int pre_posix::file::open_write_append( const std::string& fname )
{
    mode_t mode = 0664;

    return call_check_open( fname, O_WRONLY|O_CREAT|O_APPEND, mode );

    //  Флаг O_SYNC дает бешеную задержку, ну и ладно с ним.
    //return call_check_open( fname, O_WRONLY|O_CREAT|O_APPEND|O_SYNC, mode );
}
//=======================================================================================
off_t pre_posix::file::size( const string& fname )
{
    struct stat buffer;
    call_check_stat( fname.c_str(), &buffer );
    return buffer.st_size;
}
//=======================================================================================
void pre_posix::file::write(int fd, const std::string &data)
{
    ssize_t res = call_check( ::write, fd, data.c_str(), data.size() );

    if ( res != ssize_t(data.size()) )
        throw verror << "Not all data was written";
}
//=======================================================================================
void pre_posix::file::close( int fd )
{
    if ( fd < 0 ) return;
    call_check( ::close, fd );
}
//=======================================================================================
//  см. [1]
bool pre_posix::file::exists( const std::string& fname )
{
    struct stat buffer;
    return 0 == call_no_err( ::stat, fname.c_str(), &buffer );
}
//=======================================================================================
void pre_posix::file::rename_safe( const std::string& was, const std::string& will )
{
    //  Эта магия для промышленной отработки захвата ссылок на нужные методы.
    // Точно пригодится в remove (т.к. там будет конфликт с 17-м Стандартом).
    using rename_type = int(&)(const char *, const char *);
    constexpr auto& rename_ref = static_cast<rename_type>(std::rename);

    call_no_err( rename_ref, was.c_str(), will.c_str() );
}
//=======================================================================================
//  Банально ищем все '/' и пытаемся создать такой каталог.
void pre_posix::file::make_path( const std::string& path )
{
    //  Здесь нет опечатки, в заголовке цикла нет условия выхода, оно проверяется в конце
    //  цикла, чтобы пройтись и по итератору path.end().
    for( auto it = path.begin(); ; ++it )
    {
        if ( it != path.end() && *it != '/' ) continue;

        string cur_dir{ path.begin(), it };
        mode_t mode = 0755; // [2]
        call_no_err( ::mkdir, cur_dir.c_str(), mode );

        if ( it == path.end() ) break; // для обработки создания самого пути.
    }
}
//=======================================================================================
