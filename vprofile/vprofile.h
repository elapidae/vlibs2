#ifndef VPROFILE_H
#define VPROFILE_H

#include "impl_vprofile/profile.h"

#ifdef V_NEED_PROFILE
    #define V_PROFILE           static const int _v_profile_id = \
                                            impl_vprofile::profile::_register_id( \
                                            __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
                                impl_vprofile::profile _v_profile_point( _v_profile_id )
#else
    #define V_PROFILE
#endif


#endif // VPROFILE_H
