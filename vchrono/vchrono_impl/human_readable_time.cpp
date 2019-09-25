#include "human_readable_time.h"

#include <assert.h>
#include <stdexcept>
#include "vvoid_type.h"
#include "vcompiler_traits.h"

#include <sstream>
#include <iomanip>

using namespace impl;


//=======================================================================================
// Милый mktime считает, что время локальное, не UTC.
// Еле нашел решение (которое "- timezone"):
// http://qaru.site/questions/221156/stdmktime-and-timezone-info
static time_t tm_to_time_t( tm * tm )
{
    return std::mktime(tm) - timezone;
}
//=======================================================================================
static std::string str_ms( int ms )
{
    std::ostringstream res;
    res << '.' << std::setfill('0') << std::setw(3) << ms;
    return res.str();
}
//=======================================================================================


//=======================================================================================
human_readable_time human_readable_time::from_format( const std::string& dt,
                                                      const std::string& fmt )
{
    //      Alternate variant, but do not work on gcc 4.8
    //    std::tm t = {};
    //    std::istringstream ss( dt );
    //    ss >> std::get_time( &t, fmt.c_str() );
    //    if ( ss.fail() || ss.bad() )
    //        throw std::runtime_error( "human_readable_time: bad date '" + dt +
    //                                  "' with fmt '" + fmt + "'" );

    std::tm t = {};
    auto end_pos = strptime( dt.c_str(), fmt.c_str(), &t );
    uint distance = uint(end_pos - dt.c_str());
    if ( distance != dt.size() )
        throw std::runtime_error( "human_readable_time: bad date '" + dt +
                                  "' with fmt '" + fmt + "'" );
    return tm_to_time_t( &t );
}
//=======================================================================================
human_readable_time human_readable_time::from_date_time( const std::string& dt )
{
    return from_format( dt, "%Y-%m-%d %H:%M:%S" );
}
//=======================================================================================
human_readable_time human_readable_time::from_iso8601( const std::string& dt )
{
    return from_format( dt, "%Y-%m-%dT%H:%M:%S" );
}
//=======================================================================================
V_NORETURN
static void th_err( std::string msg )
{
    throw std::runtime_error( "impl::human_readable_time: " + msg );
}

human_readable_time human_readable_time::from_utc( int year, int month,  int day,
                                                   int hour, int minute, int sec )
{
    if ( year   < 1900 )             th_err( "bad year"     );
    if ( month  < 1 || month  > 12 ) th_err( "bad year"     );
    if ( day    < 1 || day    > 31 ) th_err( "bad day"      );
    if ( hour   < 0 || hour   > 23 ) th_err( "bad hour"     );
    if ( minute < 0 || minute > 59 ) th_err( "bad minute"   );
    if ( sec    < 0 || sec    > 59 ) th_err( "bad second"   );

    tm t{};
    t.tm_year = year - 1900;
    t.tm_mon  = month - 1;
    t.tm_mday = day;
    t.tm_hour = hour;
    t.tm_min  = minute;
    t.tm_sec  = sec;

    return tm_to_time_t( &t );
}
//=======================================================================================


//=======================================================================================
human_readable_time::human_readable_time( time_t tt, uint millisec )
    : _milliseconds( int(millisec) )
{
    assert( millisec < 1000 );
    gmtime_r( &tt, &_tm );
}
//---------------------------------------------------------------------------------------
int human_readable_time::year() const
{
    return _tm.tm_year + 1900;
}
//---------------------------------------------------------------------------------------
int human_readable_time::month() const
{
    return _tm.tm_mon + 1;
}
//---------------------------------------------------------------------------------------
int human_readable_time::day() const
{
    return _tm.tm_mday;
}
//---------------------------------------------------------------------------------------
int human_readable_time::hour() const
{
    return _tm.tm_hour;
}
//---------------------------------------------------------------------------------------
int human_readable_time::minute() const
{
    return _tm.tm_min;
}
//---------------------------------------------------------------------------------------
int human_readable_time::second() const
{
    return _tm.tm_sec;
}
//---------------------------------------------------------------------------------------
int human_readable_time::millisecond() const
{
    return _milliseconds;
}
//=======================================================================================

//=======================================================================================
std::string human_readable_time::format( const std::string& fmt ) const
{
    char res[257];
    auto sz = std::strftime( res, sizeof(res), fmt.c_str(), &_tm );
    return std::string( res, sz );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::iso8601() const
{
    return format( "%Y-%m-%dT%H:%M:%S" );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::iso8601_zzz() const
{
    return iso8601() + str_ms(_milliseconds);
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::date_time() const
{
    return format( "%Y-%m-%d %H:%M:%S" );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::date_time_zzz() const
{
    return date_time() + str_ms(_milliseconds);
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::date() const
{
    return format( "%Y-%m-%d" );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::time() const
{
    return format( "%H:%M:%S" );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::time_zzz() const
{
    return time() + str_ms( millisecond() );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::datetime_for_filename() const
{
    return format( "%Y-%m-%d_T_%H_%M_%S" );
}
//---------------------------------------------------------------------------------------
std::string human_readable_time::datetime_zzz_for_filename() const
{
    return datetime_for_filename() + str_ms( millisecond() );
}
//=======================================================================================

//=======================================================================================
time_t human_readable_time::_to_time_t() const
{
    auto tm_copy = _tm;
    return tm_to_time_t( &tm_copy );
}
//=======================================================================================
