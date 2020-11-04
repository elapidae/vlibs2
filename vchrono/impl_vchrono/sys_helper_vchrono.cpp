//  gmtime has ugly implementations
//  https://en.cppreference.com/w/c/chrono/gmtime

#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>

#include <stdexcept>

#include "sys_helper_vchrono.h"

#include "vcompiler_traits.h"

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
#if defined __USE_POSIX || __GLIBC_USE (ISOC2X)
void impl_vchrono::sys_helper::time_t_to_tm( time_t tt, tm *res )
{
    auto res_ptr = gmtime_r( &tt, res );
    if ( !res_ptr )
        throw std::runtime_error( "Cannot convert time_t(" +
                                  std::to_string(tt) + ") to tm" );
}
#else
//---------------------------------------------------------------------------------------
//  В WinAPI gmtime_s имеет другой синтаксис, нежели в стандарте.
//  Будем проверять что оно возвращает и, в зависимости, перегружать
//  функцию do_time_t_to_tm
using Ret_gmtime_s = decltype( gmtime_s(0,0) );
//---------------------------------------------------------------------------------------
std::enable_if<std::is_same<Ret_gmtime_s,errno_t>::value, void>::type
static do_time_t_to_tm( time_t tt, tm *res )
{
    auto err = gmtime_s( res, &tt );
    if ( err )
        throw std::runtime_error( "Cannot convert time_t(" +
                                  std::to_string(tt) + ") to tm" );
}
//---------------------------------------------------------------------------------------
//  MinGW, походу, вообще не умеет в SFINAE, банним до лучших времен.
//
//std::enable_if<std::is_same<Ret_gmtime_s,std::tm*>::value, void>::type
//static do_time_t_to_tm( time_t tt, tm *res )
//{
//    auto err = gmtime_s( res, &tt );
//    if ( err )
//        throw std::runtime_error( "Cannot convert time_t(" +
//                                  std::to_string(tt) + ") to tm" );
//}
//---------------------------------------------------------------------------------------
void impl_vchrono::sys_helper::time_t_to_tm( time_t tt, tm *res )
{
    do_time_t_to_tm( tt, res );
}
#endif // gmtime sex.
//---------------------------------------------------------------------------------------
#ifdef NEED_UNDEF_GLIBC_USE
    #undef NEED_UNDEF_GLIBC_USE
    #undef __GLIBC_USE
#endif // GLIBC_USE macro sex.
//=======================================================================================
