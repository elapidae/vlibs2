#ifndef VTIME_POINT_H
#define VTIME_POINT_H

//=======================================================================================
/**
 *      Группа классов - оберток вокруг std::chrono::*_clock.
 *
 *  Предоставляется три класса, соответствующих введенным в 11 Стандарте:
 *      class vsystem_time_point
 *      class vsteady_time_point
 *      class vhigh_resolution_time_point
 *      using vtime_point = vsystem_time_point // по умолчанию -- системное время.
 *
 *  Все они предоставляют одинаковый интерфейс,
 *  читать его не очень приятно, попробую описать API (на примере VTimePoint):
 *
 *  NB! Класс не заточен для работы с локальным временем. Подразумевается UTC время.
 *      Если будут запросы по работе с локальным представлением времени, необходимо будет
 *      продумать API. Использовать API локали не очень хочется...
 *
 *  (ts -- сокращение от timestamp, подразумевает созданный экземпляр)
 *
 *      vtime_point::now() // статический метод захвата текущего времени.
 *
 *  // стандарные вхождения:
 *      [as in std::chrono] time_point();
 *      time_t to_time_t();
 *
 *      year(), month(), day(), hour(), minute(), second()       // int типы.
 *      nanoseconds(), microseconds(), milliseconds(), seconds() // std::chrono типы.
 *
 *  Операторы сравнения.
 *  NB! Если какие-нибудь сравнения не будут действовать -- сообщите автору!
 *
 *  Есть операторы вычитания, т.е. можно писать:
 *      auto ts = VTimePoint::now();
 *      .... // long code
 *      auto diff_ts = VTimePoint::now() - ts;
 *      ts -= VTimePoint::now();
 *
 *  TODO: пока не введены операторы суммирования (не уверен, что они нужны).
 *
 *  Преобразования в строки:
 *      humanable().datetime()                      // yyyy-MM-dd hh:mm:ss
 *      humanable().datetime_zzz()                  // yyyy-MM-dd hh:mm:ss.zzz
 *      humanable().date()                          // yyyy-MM-dd
 *      humanable().time()                          // hh:mm:ss
 *      humanable().time_zzz()                      // hh:mm:ss.zzz
 *      humanable().datetime_for_filenames()        // yyyy-MM-dd_T_hh_mm_ss
 *      humanable().datetime_zzz_for_filenames()    // yyyy-MM-dd_T_hh_mm_ss.zzz
 *
 *      // Произвольный формат.
 *      // Используется функция strftime, размер результата не должен превышать 256 байт.
 *      // http://en.cppreference.com/w/cpp/chrono/c/strftime
 *      std::string str_format( const std::string &fmt ) const;
 *
 *  Не касается класса непосредственно:
 *  Перегружен стандартный вывод для типов std::chrono::[milli/micro/nano]seconds.
 *  Т.е. после включения заголовка (#include "vtimepoint.h") можно писать так:
 *      std::cout << VTimePoint::now().milliseconds();
 *      // то же самое.
 *      std::cout << std::chrono::duration_cast<std::chrono::milliseconds>
 *                                                    (std::chrono::system_clock::now());
**/
//=======================================================================================

#include "impl_vchrono/time_point_base.h"
#include <ostream>

//=======================================================================================
class vsystem_time_point
                : public impl_vchrono::time_point_base< std::chrono::system_clock,
                                                        vsystem_time_point >
{
    using _base_type = impl_vchrono::time_point_base< std::chrono::system_clock,
                                                      vsystem_time_point >;
public:
    vsystem_time_point() : _base_type() {}
    vsystem_time_point( const system_type& tp ) : _base_type(tp) {}

    template<typename Duration>
    vsystem_time_point( const Duration& d ) : _base_type(d) {}
};

using vtime_point = vsystem_time_point;

//=======================================================================================
class vsteady_time_point
        : public impl_vchrono::time_point_base< std::chrono::steady_clock,
                                                vsteady_time_point >
{
    using _base_type = impl_vchrono::time_point_base< std::chrono::steady_clock,
                                                      vsteady_time_point >;
public:
    vsteady_time_point() : _base_type() {}
    vsteady_time_point( const system_type& tp ) : _base_type(tp) {}

    template<typename Duration>
    vsteady_time_point( const Duration& d ) : _base_type(d) {}
};
//=======================================================================================
class vhigh_resolution_time_point
        : public impl_vchrono::time_point_base< std::chrono::high_resolution_clock,
                                                vhigh_resolution_time_point >
{
    using _base_type = impl_vchrono::time_point_base< std::chrono::high_resolution_clock,
                                                      vhigh_resolution_time_point >;
public:
    vhigh_resolution_time_point() : _base_type() {}
    vhigh_resolution_time_point( const system_type& tp ) : _base_type(tp) {}

    template<typename Duration>
    vhigh_resolution_time_point( const Duration& d ) : _base_type(d) {}
};
//=======================================================================================


//=======================================================================================
//      Streaming for my and std types.
//=======================================================================================
namespace std
{
    ostream & operator << ( ostream & os, const chrono::hours        & val );
    ostream & operator << ( ostream & os, const chrono::minutes      & val );
    ostream & operator << ( ostream & os, const chrono::seconds      & val );
    ostream & operator << ( ostream & os, const chrono::milliseconds & val );
    ostream & operator << ( ostream & os, const chrono::microseconds & val );
    ostream & operator << ( ostream & os, const chrono::nanoseconds  & val );
} // std namespace
//=======================================================================================
//      Streaming for std types.
//=======================================================================================

#endif // VTIME_POINT_H
