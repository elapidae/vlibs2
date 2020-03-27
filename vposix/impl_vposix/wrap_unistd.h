#ifndef IMPL_VPOSIX_WRAP_UNISTD_H
#define IMPL_VPOSIX_WRAP_UNISTD_H

//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man0/unistd.h.0p.html
 *
 *  http://man7.org/linux/man-pages/man2/pipe.2.html
 *  http://man7.org/linux/man-pages/man2/dup.2.html
 *  http://man7.org/linux/man-pages/man2/read.2.html
 *  http://man7.org/linux/man-pages/man2/execve.2.html
 *  http://man7.org/linux/man-pages/man2/wait.2.html
 *  http://man7.org/linux/man-pages/man2/getpid.2.html
 *
 *  Список функций см. ниже определения, он, таки, большой.
 *
*/
//=======================================================================================

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_unistd final
    {
        //-------------------------------------------------------------------------------
        static void close( int fd );

        static void usleep( uint usec );

        static void write( int fd, const std::string& data );

        static constexpr size_t _read_buf_size = 32768; // changed same as Qt sources...
        static std::string read( int fd );

        static ssize_t read( int fd, void *buf, size_t buf_size );
        static ssize_t read_no_err( int fd, void *buf, size_t buf_size );

        static bool unlink_no_err( const std::string& filename );

        //-------------------------------------------------------------------------------
        static void pipe_non_block( int fds[2] ); // do non blocking
        static void pipe( int fds[2] ); // do as default

        static void dup2( int oldfd, int newfd );
        static int  dup( int clone_fd );
        static int  fork();
        static void exec( const char *cmd, const char * const *argv );
        //-------------------------------------------------------------------------------
        static pid_t get_pid()                noexcept;
        static pid_t get_parent_pid()         noexcept;
        static pid_t get_pid_group()          noexcept;
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

/*
Declarations
      The following shall be declared as functions and may also be defined
      as macros. Function prototypes shall be provided.

          int          access(const char *, int);
          unsigned     alarm(unsigned);
          int          chdir(const char *);
          int          chown(const char *, uid_t, gid_t);
          int          close(int);
          size_t       confstr(int, char *, size_t);
          char        *crypt(const char *, const char *);
          int          dup(int);
          int          dup2(int, int);
          void         _exit(int);
          void         encrypt(char [64], int);
          int          execl(const char *, const char *, ...);
          int          execle(const char *, const char *, ...);
          int          execlp(const char *, const char *, ...);
          int          execv(const char *, char *const []);
          int          execve(const char *, char *const [], char *const []);
          int          execvp(const char *, char *const []);
          int          faccessat(int, const char *, int, int);
          int          fchdir(int);
          int          fchown(int, uid_t, gid_t);
          int          fchownat(int, const char *, uid_t, gid_t, int);
          int          fdatasync(int);
          int          fexecve(int, char *const [], char *const []);
          pid_t        fork(void);
          long         fpathconf(int, int);
          int          fsync(int);
          int          ftruncate(int, off_t);
          char        *getcwd(char *, size_t);
          gid_t        getegid(void);
          uid_t        geteuid(void);
          gid_t        getgid(void);
          int          getgroups(int, gid_t []);
          long         gethostid(void);
          int          gethostname(char *, size_t);
          char        *getlogin(void);
          int          getlogin_r(char *, size_t);
          int          getopt(int, char * const [], const char *);
          pid_t        getpgid(pid_t);
          pid_t        getpgrp(void);
          pid_t        getpid(void);
          pid_t        getppid(void);
          pid_t        getsid(pid_t);
          uid_t        getuid(void);
          int          isatty(int);
          int          lchown(const char *, uid_t, gid_t);
          int          link(const char *, const char *);
          int          linkat(int, const char *, int, const char *, int);
          int          lockf(int, int, off_t);
          off_t        lseek(int, off_t, int);
          int          nice(int);
          long         pathconf(const char *, int);
          int          pause(void);
          int          pipe(int [2]);
          ssize_t      pread(int, void *, size_t, off_t);
          ssize_t      pwrite(int, const void *, size_t, off_t);
          ssize_t      read(int, void *, size_t);
          ssize_t      readlink(const char *restrict, char *restrict, size_t);
          ssize_t      readlinkat(int, const char *restrict, char *restrict, size_t);
          int          rmdir(const char *);
          int          setegid(gid_t);
          int          seteuid(uid_t);
          int          setgid(gid_t);
          int          setpgid(pid_t, pid_t);
          pid_t        setpgrp(void);
          int          setregid(gid_t, gid_t);
          int          setreuid(uid_t, uid_t);
          pid_t        setsid(void);
          int          setuid(uid_t);
          unsigned     sleep(unsigned);
          void         swab(const void *restrict, void *restrict, ssize_t);
          int          symlink(const char *, const char *);
          int          symlinkat(const char *, int, const char *);
          void         sync(void);
          long         sysconf(int);
          pid_t        tcgetpgrp(int);
          int          tcsetpgrp(int, pid_t);
          int          truncate(const char *, off_t);
          char        *ttyname(int);
          int          ttyname_r(int, char *, size_t);
          int          unlink(const char *);
          int          unlinkat(int, const char *, int);
          ssize_t      write(int, const void *, size_t);
*/

#endif // IMPL_VPOSIX_WRAP_UNISTD_H
