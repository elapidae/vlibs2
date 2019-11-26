#ifndef WRAP_STDIO_H
#define WRAP_STDIO_H


//=======================================================================================
/*
 *  http://man7.org/linux/man-pages/man0/stdio.h.0p.html
 *
 *  Список функций см. ниже определения, он, таки, большой.
*/
//=======================================================================================

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct wrap_stdio final
    {
        //-------------------------------------------------------------------------------
        static void file_rename( const std::string& was, const std::string& will );
        //-------------------------------------------------------------------------------
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

/*
       The following shall be declared as functions and may also be defined
       as macros. Function prototypes shall be provided.

           void     clearerr(FILE *);
           char    *ctermid(char *);
           int      dprintf(int, const char *restrict, ...)
           int      fclose(FILE *);
           FILE    *fdopen(int, const char *);
           int      feof(FILE *);
           int      ferror(FILE *);
           int      fflush(FILE *);
           int      fgetc(FILE *);
           int      fgetpos(FILE *restrict, fpos_t *restrict);
           char    *fgets(char *restrict, int, FILE *restrict);
           int      fileno(FILE *);
           void     flockfile(FILE *);
           FILE    *fmemopen(void *restrict, size_t, const char *restrict);
           FILE    *fopen(const char *restrict, const char *restrict);
           int      fprintf(FILE *restrict, const char *restrict, ...);
           int      fputc(int, FILE *);
           int      fputs(const char *restrict, FILE *restrict);
           size_t   fread(void *restrict, size_t, size_t, FILE *restrict);
           FILE    *freopen(const char *restrict, const char *restrict,
                        FILE *restrict);
           int      fscanf(FILE *restrict, const char *restrict, ...);
           int      fseek(FILE *, long, int);
           int      fseeko(FILE *, off_t, int);
           int      fsetpos(FILE *, const fpos_t *);
           long     ftell(FILE *);
           off_t    ftello(FILE *);
           int      ftrylockfile(FILE *);
           void     funlockfile(FILE *);
           size_t   fwrite(const void *restrict, size_t, size_t, FILE *restrict);
           int      getc(FILE *);
           int      getchar(void);
           int      getc_unlocked(FILE *);
           int      getchar_unlocked(void);
           ssize_t  getdelim(char **restrict, size_t *restrict, int,
                        FILE *restrict);
           ssize_t  getline(char **restrict, size_t *restrict, FILE *restrict);
           char    *gets(char *);
           FILE    *open_memstream(char **, size_t *);
           int      pclose(FILE *);
           void     perror(const char *);
           FILE    *popen(const char *, const char *);
           int      printf(const char *restrict, ...);
           int      putc(int, FILE *);
           int      putchar(int);
           int      putc_unlocked(int, FILE *);
           int      putchar_unlocked(int);
           int      puts(const char *);
           int      remove(const char *);
           int      rename(const char *, const char *);
           int      renameat(int, const char *, int, const char *);
           void     rewind(FILE *);
           int      scanf(const char *restrict, ...);
           void     setbuf(FILE *restrict, char *restrict);
           int      setvbuf(FILE *restrict, char *restrict, int, size_t);
           int      snprintf(char *restrict, size_t, const char *restrict, ...);
           int      sprintf(char *restrict, const char *restrict, ...);
           int      sscanf(const char *restrict, const char *restrict, ...);
           char    *tempnam(const char *, const char *);
           FILE    *tmpfile(void);
           char    *tmpnam(char *);
           int      ungetc(int, FILE *);
           int      vdprintf(int, const char *restrict, va_list);
           int      vfprintf(FILE *restrict, const char *restrict, va_list);
           int      vfscanf(FILE *restrict, const char *restrict, va_list);
           int      vprintf(const char *restrict, va_list);
           int      vscanf(const char *restrict, va_list);
           int      vsnprintf(char *restrict, size_t, const char *restrict,
                        va_list);
           int      vsprintf(char *restrict, const char *restrict, va_list);
           int      vsscanf(const char *restrict, const char *restrict, va_list);

       Inclusion of the <stdio.h> header may also make visible all symbols
       from <stddef.h>.

       The following sections are informative.
*/

#endif // WRAP_STDIO_H
