#ifndef VPROFILE_H
#define VPROFILE_H

//#include "impl_vprofile/profile.h"

#ifdef V_NEED_PROFILE
    #define V_PROFILE           static const int _v_profile_id = \
                                            vprofile::_register_id( \
                                            __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
                                vprofile _v_profile_point( _v_profile_id )
#else
    #define V_PROFILE
#endif

#include <vector>
#include <chrono>
#include <ostream>

class vprofile final
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

    vprofile( int id );
    ~vprofile();

    static int  _register_id( const char *file, const char *func, int line );
    static void _append( int id, const clock::duration& dur );

private:
    int _id;
    clock::time_point _from;
};

std::ostream& operator << ( std::ostream& os, const vprofile::point& p );


#endif // VPROFILE_H
