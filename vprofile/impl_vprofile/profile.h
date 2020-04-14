#ifndef IMPL_VPROFILE_PROFILE_H
#define IMPL_VPROFILE_PROFILE_H

#include <vector>
#include <chrono>
#include <ostream>

namespace impl_vprofile
{

    class profile final
    {
    public:
        using clock = std::chrono::steady_clock;

        struct point
        {
            using vector = std::vector<point>;

            int             id;
            const char     *file;
            const char     *func;
            int             line;
            clock::duration duration {};
        };

        static point::vector current();

        profile( int id );
        ~profile();

        static int  _register_id( const char *file, const char *func, int line );
        static void _append( int id, const clock::duration& dur );

    private:
        int _id;
        clock::time_point _from;
    };

    std::ostream& operator << (std::ostream& os, const profile::point& p );
} // namespace impl_vprofile

#endif // IMPL_VPROFILE_PROFILE_H
