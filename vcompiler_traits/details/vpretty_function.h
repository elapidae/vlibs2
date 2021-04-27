#ifndef VSIMPLE_VPRETTY_FUNCTION_H
#define VSIMPLE_VPRETTY_FUNCTION_H

#include "details/vplatform.h"

#if( defined(V_COMPILER_MINGW) || defined(V_COMPILER_GNU) )

    #define V_PRETTY_FUNCTION __PRETTY_FUNCTION__

#elif( defined(V_COMPILER_MSVC) )

    #define V_PRETTY_FUNCTION __FUNCSIG__
#else
    //  It is the 11th stantard.
    #define V_PRETTY_FUNCTION __func__
#endif

#endif // VSIMPLE_VPRETTY_FUNCTION_H
