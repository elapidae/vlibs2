/****************************************************************************************
**
**  VLIBS codebase, NIIAS
**
**  GNU Lesser General Public License Usage
**  This file may be used under the terms of the GNU Lesser General Public License
**  version 3 as published by the Free Software Foundation and appearing in the file
**  LICENSE.LGPL3 included in the packaging of this file. Please review the following
**  information to ensure the GNU Lesser General Public License version 3 requirements
**  will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
****************************************************************************************/

#ifndef IMPL_VCHRONO_TIME_METER_H
#define IMPL_VCHRONO_TIME_METER_H

#include <chrono>

//=======================================================================================
namespace impl_vchrono
{
    //===================================================================================
    template<typename Clk>
    class time_meter
    {
    public:

        using time_point = typename Clk::time_point;
        using duration   = typename Clk::duration;

        class Duration : public duration
        {
        public:
            Duration( const duration& rhs );

            std::chrono::seconds       sec() const;
            std::chrono::milliseconds  ms()  const;
            std::chrono::microseconds  us()  const;
            std::chrono::nanoseconds   ns()  const;
        };

        time_meter();

        enum _stopped { stopped };
        time_meter( _stopped );

        Duration elapsed() const;
        Duration restart();

        Duration pause();
        Duration unpause();

    private:
        duration   _elapsed {};
        time_point _catched {};
        bool       _paused;
    };
    //===================================================================================
    template<typename Clk>
    time_meter<Clk>::time_meter()
        : _catched ( Clk::now() )
        , _paused  ( false )
    {}
    //===================================================================================
    template<typename Clk>
    time_meter<Clk>::time_meter( _stopped )
        : _paused( true )
    {}
    //===================================================================================
    template<typename Clk> typename time_meter<Clk>::Duration
    time_meter<Clk>::elapsed() const
    {
        if ( _paused )
            return Duration(_elapsed);

        return _elapsed + (Clk::now() - _catched);
    }
    //===================================================================================
    template<typename Clk> typename time_meter<Clk>::Duration
    time_meter<Clk>::restart()
    {
        auto res = elapsed();
        _paused  = false;
        _elapsed = duration{};
        _catched = Clk::now();
        return res;
    }
    //===================================================================================
    template<typename Clk> typename time_meter<Clk>::Duration
    time_meter<Clk>::pause()
    {
        auto res = elapsed();
        _paused  = true;
        _elapsed = res;
        return res;
    }
    //===================================================================================
    template<typename Clk> typename time_meter<Clk>::Duration
    time_meter<Clk>::unpause()
    {
        auto res = elapsed();
        _paused  = false;
        _elapsed = res;
        _catched = Clk::now();
        return res;
    }
    //===================================================================================
    template<typename Clk>
    time_meter<Clk>::Duration::Duration( const duration& rhs )
        : duration( rhs )
    {}
    //-----------------------------------------------------------------------------------
    template<typename Clk>
    std::chrono::seconds time_meter<Clk>::Duration::sec() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(*this);
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk>
    std::chrono::milliseconds time_meter<Clk>::Duration::ms() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(*this);
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk>
    std::chrono::microseconds time_meter<Clk>::Duration::us() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(*this);
    }
    //-----------------------------------------------------------------------------------
    template<typename Clk>
    std::chrono::nanoseconds time_meter<Clk>::Duration::ns() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(*this);
    }
    //===================================================================================
} // impl_vchrono namespace
//=======================================================================================

#endif // IMPL_VCHRONO_TIME_METER_H
