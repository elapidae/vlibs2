#ifndef IMPL_VLOG_PRE_POSIX_H
#define IMPL_VLOG_PRE_POSIX_H

//=======================================================================================
/*
 *  pre posix инкапсулирует posix вызовы в необходимом и достаточном количестве для
 *  решения задач по записи логов.
 *
 *  По идее, когда vposix будет перенесена, то необходимость в этой части отпадет.
 *
 *  В случае ошибок будет бросаться исключениями.
 *
 *  Было сделано тестирование в многопоточном режиме, вроде все работает.
*/
//=======================================================================================

#include <string>

using uint = unsigned int; // For alpine compatibility.

//=======================================================================================
class pre_posix
{
public:
    //===================================================================================
    static std::string err_msg();
    //===================================================================================

    //===================================================================================
    struct file final
    {
        static int    open_write_append( const std::string& fname );
        static off_t  size( const std::string& fname );
        static bool   exists( const std::string& fname );

        static void   make_path( const std::string& path );

        static void   write( int fd, const std::string& data );
        static void   close( int fd );

        static void   rename_safe( const std::string& was, const std::string& will );

        class log_file;
        class rotate_log_file;

        static std::string start_line_for_log();
        static std::string fin_line_for_log();
        class shared_log;
        class leveled_log;
    };
    //===================================================================================
};
//=======================================================================================


#endif // IMPL_VLOG_PRE_POSIX_H
