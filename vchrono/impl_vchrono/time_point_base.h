#ifndef VCHRONO_IMPL_TIME_POINT_BASE_H
#define VCHRONO_IMPL_TIME_POINT_BASE_H

#include <chrono>
#include "impl_vchrono/human_readable_time.h"

//=======================================================================================
namespace impl_vchrono
{
    template<typename Clk, typename Derived>
    class time_point_base
    {
    public:
        using system_type = typename Clk::time_point;
        using clock_type  = Clk;

        //-------------------------------------------------------------------------------
        static Derived now();
        static bool is_steady();

        //-------------------------------------------------------------------------------
        static Derived
        from_format ( const std::string &dt, const std::string &fmt );

        static Derived
        from_date_time( const std::string &dt );    // From "yyyy-MM-dd hh:mm:ss".

        static Derived
        from_iso8601( const std::string &dt );      // From "yyyy-MM-ddThh:mm:ss".

        static Derived
        from_utc(int year, int month, int day, int hour, int minute, int sec);

        //-------------------------------------------------------------------------------
        explicit time_point_base();
        explicit time_point_base( const system_type& tp );

        template<typename Duration2>
        explicit time_point_base( const Duration2& d2 ); // Построение из duration.

        //-------------------------------------------------------------------------------
        system_type time_point() const;
        std::time_t to_time_t()  const;

        //-------------------------------------------------------------------------------
        human_readable_time humanable() const;

        //-------------------------------------------------------------------------------
        std::chrono::nanoseconds    nanoseconds()   const;
        std::chrono::microseconds   microseconds()  const;
        std::chrono::milliseconds   milliseconds()  const;
        std::chrono::seconds        seconds()       const;
        std::chrono::minutes        minutes()       const;
        std::chrono::hours          hours()         const;

        //-------------------------------------------------------------------------------
        bool operator <  ( const Derived& rhs ) const;
        bool operator >  ( const Derived& rhs ) const;
        bool operator <= ( const Derived& rhs ) const;
        bool operator >= ( const Derived& rhs ) const;
        bool operator == ( const Derived& rhs ) const;
        bool operator != ( const Derived& rhs ) const;

        //-------------------------------------------------------------------------------
        template<typename Duration> bool operator <  ( const Duration& rhs ) const;
        template<typename Duration> bool operator >  ( const Duration& rhs ) const;
        template<typename Duration> bool operator <= ( const Duration& rhs ) const;
        template<typename Duration> bool operator >= ( const Duration& rhs ) const;
        template<typename Duration> bool operator == ( const Duration& rhs ) const;
        template<typename Duration> bool operator != ( const Duration& rhs ) const;

        //-------------------------------------------------------------------------------
        template<typename Duration>
        Derived& operator += ( const Duration& rhs );

        // has not in std::chrono...
        //Derived& operator += ( const Derived&  rhs );

        template<typename Duration>
        Derived& operator -= ( const Duration& rhs );
        Derived& operator -= ( const Derived&  rhs );

        //-------------------------------------------------------------------------------
    private:
        system_type _tp{};
    };
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    Derived operator + ( time_point_base<Clk,Derived> t1,
                         const time_point_base<Clk,Derived>& t2 );
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived, typename Duration>
    Derived operator + ( const Duration& t1,
                         time_point_base<Clk,Derived> t2 );
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived, typename Duration>
    Derived operator + ( time_point_base<Clk,Derived> t1,
                         const Duration& t2 );
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    Derived operator - ( time_point_base<Clk,Derived> t1,
                         const time_point_base<Clk,Derived>& t2 );
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived, typename Duration>
    Derived operator - ( time_point_base<Clk,Derived> t1,
                         const Duration& t2 );
    //===================================================================================


    //===================================================================================
    //      Implementation
    //===================================================================================
    template<typename Clk, typename Derived>
    Derived time_point_base<Clk,Derived>::from_format ( const std::string& dt,
                                                        const std::string& fmt )
    {
        return human_readable_time::from_format( dt, fmt ).to_time_point<Derived>();
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    Derived time_point_base<Clk,Derived>::from_date_time( const std::string& dt )
    {
        return human_readable_time::from_date_time( dt ).to_time_point<Derived>();
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    Derived time_point_base<Clk,Derived>::from_iso8601( const std::string& dt )
    {
        return human_readable_time::from_iso8601( dt ).to_time_point<Derived>();
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    Derived time_point_base<Clk,Derived>::from_utc( int year, int month,  int day,
                                                    int hour, int minute, int sec)
    {
        return human_readable_time::from_utc( year, month, day, hour, minute, sec )
                                                            .to_time_point<Derived>();
    }
    //===================================================================================
    template<typename Clk, typename Derived>
    Derived time_point_base<Clk,Derived>::now()
    {
        return Derived( Clk::now() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::is_steady()
    {
        return Clk::is_steady;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    time_point_base<Clk,Derived>::time_point_base()
    {}
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    time_point_base<Clk,Derived>::time_point_base( const system_type& tp )
        : _tp( tp )
    {}
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration2>
    time_point_base<Clk,Derived>::time_point_base( const Duration2& d2 )
        : _tp( system_type(d2) )
    {}
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    typename time_point_base<Clk,Derived>::system_type
    time_point_base<Clk,Derived>::time_point() const
    {
        return _tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    std::time_t time_point_base<Clk,Derived>::to_time_t() const
    {
        return Clk::to_time_t( _tp );
    }
    //===================================================================================
    template<typename Clk, typename Derived>
    human_readable_time time_point_base<Clk,Derived>::humanable() const
    {
        return *this;
    }
    //===================================================================================
    template<typename Clk, typename Derived>
    std::chrono::nanoseconds time_point_base<Clk,Derived>::nanoseconds() const
    {
        using dur = std::chrono::nanoseconds;
        return std::chrono::duration_cast<dur>( _tp.time_since_epoch() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    std::chrono::microseconds time_point_base<Clk,Derived>::microseconds() const
    {
        using dur = std::chrono::microseconds;
        return std::chrono::duration_cast<dur>( _tp.time_since_epoch() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    std::chrono::milliseconds time_point_base<Clk,Derived>::milliseconds() const
    {
        using dur = std::chrono::milliseconds;
        return std::chrono::duration_cast<dur>( _tp.time_since_epoch() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    std::chrono::seconds time_point_base<Clk,Derived>::seconds() const
    {
        using dur = std::chrono::seconds;
        return std::chrono::duration_cast<dur>( _tp.time_since_epoch() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    std::chrono::minutes time_point_base<Clk,Derived>::minutes() const
    {
        using dur = std::chrono::minutes;
        return std::chrono::duration_cast<dur>( _tp.time_since_epoch() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    std::chrono::hours time_point_base<Clk,Derived>::hours() const
    {
        using dur = std::chrono::hours;
        return std::chrono::duration_cast<dur>( _tp.time_since_epoch() );
    }
    //===================================================================================
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::operator < ( const Derived& rhs ) const
    {
        return _tp < rhs._tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::operator > ( const Derived& rhs ) const
    {
        return _tp > rhs._tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::operator <= ( const Derived& rhs ) const
    {
        return _tp <= rhs._tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::operator >= ( const Derived& rhs ) const
    {
        return _tp >= rhs._tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::operator == ( const Derived& rhs ) const
    {
        return _tp == rhs._tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    bool time_point_base<Clk,Derived>::operator != ( const Derived& rhs ) const
    {
        return _tp != rhs._tp;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    bool time_point_base<Clk,Derived>::operator < ( const Duration& rhs ) const
    {
        return _tp < rhs;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    bool time_point_base<Clk,Derived>::operator > ( const Duration& rhs ) const
    {
        return _tp > rhs;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    bool time_point_base<Clk,Derived>::operator <= ( const Duration& rhs ) const
    {
        return _tp <= rhs;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    bool time_point_base<Clk,Derived>::operator >= ( const Duration& rhs ) const
    {
        return _tp >= rhs;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    bool time_point_base<Clk,Derived>::operator == ( const Duration& rhs ) const
    {
        return _tp.time_since_epoch() == rhs;
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    bool time_point_base<Clk,Derived>::operator != ( const Duration& rhs ) const
    {
        return _tp != rhs;
    }
    //===================================================================================
    template<typename Clk, typename Derived>
    template<typename Duration>
    Derived& time_point_base<Clk,Derived>::operator -= ( const Duration& rhs )
    {
        _tp -= rhs;
        return static_cast<Derived&>( *this );
    }
    //-----------------------------------------------------------------------------------
    template<class> class TD;
    template<typename Clk, typename Derived>
    Derived& time_point_base<Clk,Derived>::operator -= ( const Derived& rhs )
    {
        _tp = system_type( _tp - rhs._tp );
        return static_cast<Derived&>( *this );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    template<typename Duration>
    Derived& time_point_base<Clk,Derived>::operator += ( const Duration& rhs )
    {
        _tp += rhs;
        return static_cast<Derived&>( *this );
    }
    //===================================================================================

    //===================================================================================
    template<typename Clk, typename Derived>
    Derived operator + ( time_point_base<Clk,Derived> tres,
                         const time_point_base<Clk,Derived>& t2 )
    {
        tres += t2;
        return Derived( tres );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived, typename Duration>
    Derived operator + ( const Duration& d,
                         time_point_base<Clk,Derived> tres )
    {
        tres += d;
        return Derived( tres );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived, typename Duration>
    Derived operator + ( time_point_base<Clk,Derived> tres,
                         const Duration& d )
    {
        tres += d;
        return Derived( tres.time_point() );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived>
    Derived operator - ( time_point_base<Clk,Derived> tres,
                         const time_point_base<Clk,Derived>& t2 )
    {
        tres -= t2;
        return Derived( tres );
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk, typename Derived, typename Duration>
    Derived operator - ( time_point_base<Clk,Derived> tres,
                         const Duration& d )
    {
        tres -= d;
        return Derived( tres.time_point() );
    }
    //===================================================================================
} // namespace impl_vchrono
//=======================================================================================



#endif // VCHRONO_IMPL_TIME_POINT_BASE_H
