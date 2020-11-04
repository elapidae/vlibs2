#ifndef SYS_HELPER_VCHRONO_H
#define SYS_HELPER_VCHRONO_H

#include <ctime>
#include <string>

namespace impl_vchrono
{
    struct sys_helper
    {
        static std::tm str_to_tm( const std::string& dt, const std::string& fmt );

        //  May throw runtime_error.
        static void time_t_to_tm( time_t tt, struct std::tm * res );
    };
}

#endif // SYS_HELPER_VCHRONO_H
