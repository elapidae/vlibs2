#ifndef IMPL_VPOSIX_WRAP_UNISTD_H
#define IMPL_VPOSIX_WRAP_UNISTD_H

//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man0/unistd.h.0p.html
 *
 *  Список функций см. ниже определения, он, таки, большой.
 *
*/
//=======================================================================================

#include <string>
#include "impl_vposix/wrap_sys_epoll.h"

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct safe_fd final
    {
        safe_fd() noexcept;
        safe_fd( int _fd ) noexcept;
        ~safe_fd();

        void close();

        operator int() const noexcept;
        bool has() const noexcept;

        bool in_poll() const noexcept;

        safe_fd( safe_fd&& ) noexcept;
        safe_fd& operator = ( safe_fd&& ) noexcept;

        safe_fd& operator = ( int other );

        void poll_add_read   ( epoll_receiver * receiver );
        void poll_add_write  ( epoll_receiver * receiver );
        void poll_add_rw     ( epoll_receiver * receiver );

        void poll_mod_read   ( epoll_receiver * receiver );
        void poll_mod_write  ( epoll_receiver * receiver );
        void poll_mod_rw     ( epoll_receiver * receiver );

        void poll_del();

    private:
        bool _in_poll = false;
        int  _fd = -1;

        safe_fd( const safe_fd& ) = delete;
        safe_fd& operator = ( const safe_fd& ) = delete;
    };
    //===================================================================================
    struct wrap_unistd final
    {
        //-------------------------------------------------------------------------------
        static void close( int fd );
        static bool close_safe( int fd ); //     noexcept, but cancellation point.

        static void usleep( uint usec );

        static void write( int fd, const std::string& data );

        static ssize_t read( int fd, void *buf, size_t buf_size );
        static ssize_t read_no_err( int fd, void *buf, size_t buf_size );
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
