#ifndef VCHRONO_IMPL_HUMAN_READABLE_TIME_H
#define VCHRONO_IMPL_HUMAN_READABLE_TIME_H

#include <chrono>
#include <string>

//=======================================================================================
namespace impl
{
    //===================================================================================
    class human_readable_time
    {
    public:
        //-------------------------------------------------------------------------------
        template<typename TimePoint>
        TimePoint to_time_point() const;

        //-------------------------------------------------------------------------------
        static human_readable_time
        from_format ( const std::string &dt, const std::string &fmt );

        static human_readable_time
        from_date_time( const std::string &dt );    // From "yyyy-MM-dd hh:mm:ss".

        static human_readable_time
        from_iso8601( const std::string &dt );      // From "yyyy-MM-ddThh:mm:ss".

        static human_readable_time
        from_utc(int year, int month, int day, int hour, int minute, int sec);

        //-------------------------------------------------------------------------------
        template<typename time_point>
        human_readable_time( const time_point& tp )
            : human_readable_time( tp.to_time_t(), tp.milliseconds().count() % 1000 )
        {}

        human_readable_time( std::time_t tt, uint millisec = 0 );

        //-------------------------------------------------------------------------------
        int year()          const;  //
        int month()         const;  //  NB! 1..12
        int day()           const;  //  NB! 1..31
        int hour()          const;  //  NB! 0..23
        int minute()        const;  //  NB! 0..59
        int second()        const;  //  NB! 0..59
        int millisecond()   const;  //  NB! 0..999

        //-------------------------------------------------------------------------------
        // Используется функцией strftime, размер результата не более 256 байт.
        // http://en.cppreference.com/w/cpp/chrono/c/strftime
        std::string format( const std::string &fmt ) const;

        //-------------------------------------------------------------------------------
        std::string iso8601()                       const;
        std::string iso8601_zzz()                   const;

        std::string date_time()                     const;
        std::string date_time_zzz()                 const;

        std::string date()                          const;

        std::string time()                          const;
        std::string time_zzz()                      const;

        std::string datetime_for_filename()         const;
        std::string datetime_zzz_for_filename()     const;

        //-------------------------------------------------------------------------------
    private:
        std::tm _tm;
        int _milliseconds = 0;

        std::time_t _to_time_t() const;
    };
    //===================================================================================
    template<typename TimePoint>
    TimePoint human_readable_time::to_time_point() const
    {
        return TimePoint( TimePoint::clock_type::from_time_t(_to_time_t())
                          + std::chrono::milliseconds(_milliseconds) );
    }
    //===================================================================================
} // namespace impl
//=======================================================================================

#endif // VCHRONO_IMPL_HUMAN_READABLE_TIME_H
