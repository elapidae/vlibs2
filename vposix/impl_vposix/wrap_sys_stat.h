#ifndef IMPL_VPOSIX_WRAP_SYS_STAT_H
#define IMPL_VPOSIX_WRAP_SYS_STAT_H

//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man0/sys_stat.h.0p.html
 *
 *  Список функций см. ниже определения, он, таки, большой.
*/
//=======================================================================================

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_sys_stat final
    {
        //-------------------------------------------------------------------------------
        static off_t  file_size   ( const std::string& fname );
        static bool   file_exists ( const std::string& fname );

        //  Не проверяет права доступа, просто пытается создать все подряд папки в пути.
        static void   make_path ( const std::string& path );
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

/*
 *  The following shall be declared as functions and may also be defined
 *     as macros. Function prototypes shall be provided.
 *
 *         int    chmod(const char *, mode_t);
 *         int    fchmod(int, mode_t);
 *         int    fchmodat(int, const char *, mode_t, int);
 *         int    fstat(int, struct stat *);
 *         int    fstatat(int, const char *restrict, struct stat *restrict, int);
 *         int    futimens(int, const struct timespec [2]);
 *         int    lstat(const char *restrict, struct stat *restrict);
 *         int    mkdir(const char *, mode_t);
 *         int    mkdirat(int, const char *, mode_t);
 *         int    mkfifo(const char *, mode_t);
 *         int    mkfifoat(int, const char *, mode_t);
 *         int    mknod(const char *, mode_t, dev_t);
 *         int    mknodat(int, const char *, mode_t, dev_t);
 *         int    stat(const char *restrict, struct stat *restrict);
 *         mode_t umask(mode_t);
 *         int    utimensat(int, const char *, const struct timespec [2], int);
*/

#endif // IMPL_VPOSIX_WRAP_SYS_STAT_H
