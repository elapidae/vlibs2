//  gmtime has ugly implementations
//  https://en.cppreference.com/w/c/chrono/gmtime

#include <stdexcept>

#include "sys_helper_vchrono.h"
#include "vcompiler_traits.h"

#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>

//=======================================================================================
#if V_HAS_STD_GET_TIME
    #include <sstream>
    #include <iomanip>
    std::tm impl_vchrono::sys_helper::str_to_tm( const std::string& dt,
                                                 const std::string& fmt )
    {
        std::tm res = {};
        std::istringstream ss( dt );
        ss >> std::get_time( &res, fmt.c_str() );
        if ( ss.fail() || ss.bad() )
            throw std::runtime_error( "Bad date '" + dt + "' with format '" + fmt + "'");
        return res;
    }
#else
    std::tm impl_vchrono::sys_helper::str_to_tm( const std::string& dt,
                                                 const std::string& fmt )
    {
        std::tm res {};
        auto end_pos = strptime( dt.c_str(), fmt.c_str(), &res );
        unsigned distance = unsigned(end_pos - dt.c_str());
        if ( distance != dt.size() )
            throw std::runtime_error( "Bad date '" + dt + "' with format '" + fmt + "'");

        return res;
    }
#endif
//=======================================================================================


//  В WinAPI макрос __GLIBC_USE не определен и проверка с посиксом не работает.
//  Определим его, а потом сразу же удалим.
#ifndef __GLIBC_USE
    #define NEED_UNDEF_GLIBC_USE
    #define __GLIBC_USE(x) 0
#endif
//---------------------------------------------------------------------------------------
//  Флаги выдернуты из time.h
//  UPD 2023-02-23, added _GNU_SOURCE for alpine.
#if defined(__USE_POSIX) || (__GLIBC_USE(ISOC2X)) || defined(_GNU_SOURCE)
void impl_vchrono::sys_helper::time_t_to_tm( time_t tt, tm *res )
{
    auto res_ptr = V_GMTIME_SAFE( &tt, res );
    if ( !res_ptr )
        throw std::runtime_error( "Cannot convert time_t(" +
                                  std::to_string(tt) + ") to tm" );
}
#else
//---------------------------------------------------------------------------------------
//  В WinAPI gmtime_s имеет другой синтаксис, нежели в стандарте.
void impl_vchrono::sys_helper::time_t_to_tm( time_t tt, tm *res )
{
    auto err = V_GMTIME_SAFE( res, &tt );
    if ( err )
        throw std::runtime_error( "Cannot convert time_t(" +
                                  std::to_string(tt) + ") to tm" );
}
#endif // gmtime sex.
//---------------------------------------------------------------------------------------
#ifdef NEED_UNDEF_GLIBC_USE
    #undef NEED_UNDEF_GLIBC_USE
    #undef __GLIBC_USE
#endif // GLIBC_USE macro sex.
//=======================================================================================
