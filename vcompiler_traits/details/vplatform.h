#ifndef VSIMPLE_VPLATFORM_H
#define VSIMPLE_VPLATFORM_H

//=======================================================================================
/*
*   Define platform and compiler by macroses.
*
*   Can only some compilers...
*
*   V_COMPILER_NAME = GNU/MINGW/MSVC
*   V_COMPILER_NAME_STR as literal
*   V_COMPILER_[GNU/MINGW/MSVC]
*
*   V_PLATFORM_NAME = LINUX/WINDOWS
*   V_PLATFORM_<LINUX/WINDOWS>
*/
//=======================================================================================


//=======================================================================================
//  Compiler type
//  https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
#if ( defined(__GNUG__) )

    #if ( defined(__MINGW32__) || defined(__MINGW64__) )
        #define V_COMPILER_NAME MINGW
        #define V_COMPILER_NAME_STR "MINGW"
        #define V_COMPILER_MINGW
    #else
        #define V_COMPILER_NAME GNU
        #define V_COMPILER_NAME_STR "GNU"
        #define V_COMPILER_GNU
    #endif

// https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=msvc-160
#elif ( defined(_MSVC_LANG) )

    #define V_COMPILER_NAME MSVC
    #define V_COMPILER_NAME_STR "MSVC"
    #define V_COMPILER_MSVC

#else
    #error "Unknown compiler type"
#endif // define compiler name

//  Compiler type
//=======================================================================================
//  Platform type

#if ( defined(V_COMPILER_MSVC) || defined(V_COMPILER_MINGW) )

    #define V_PLATFORM_NAME WINDOWS
    #define V_PLATFORM_NAME_STR "WINDOWS"
    #define V_PLATFORM_WINDOWS

#elif ( defined(linux)      || \
        defined(__linux)    || \
        defined(__linux__) )

    #define V_PLATFORM_NAME LINUX
    #define V_PLATFORM_NAME_STR "LINUX"
    #define V_PLATFORM_LINUX

#else
    #error "Unknown platform type"
#endif // define platform type

//  Platform type
//=======================================================================================

#endif // VSIMPLE_VPLATFORM_H
