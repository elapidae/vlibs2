#ifndef VTIMER_H
#define VTIMER_H

#include <chrono>
#include <memory>
#include "vsignal.h"

//=======================================================================================
class vtimer final
{
public:
    explicit vtimer();

    template<typename Duration>
    explicit vtimer( const Duration& d );

    ~vtimer();

    vsignal<> timeout;                  // просто сигнал,
    vsignal<uint64_t> timeout_times;    // передает сколько интервалов прошло.

    void start      ( const std::chrono::nanoseconds& ns );
    void singleshot ( const std::chrono::nanoseconds& ns );

    void stop();

    template<typename Duration> void start      ( const Duration& d );
    template<typename Duration> void singleshot ( const Duration& d );

private:
    class _pimpl;
    std::unique_ptr<_pimpl> _p;
};
//=======================================================================================
//      Implementation
//=======================================================================================
template<typename Duration>
vtimer::vtimer( const Duration& d )
    : vtimer()
{
    start( d );
}
//=======================================================================================
template<typename Duration>
void vtimer::start( const Duration& d )
{
    using namespace std::chrono;
    start( duration_cast<nanoseconds>(d) );
}
//=======================================================================================
template<typename Duration>
void vtimer::singleshot ( const Duration& d )
{
    using namespace std::chrono;
    singleshot( duration_cast<nanoseconds>(d) );
}
//=======================================================================================


#endif // VTIMER_H
