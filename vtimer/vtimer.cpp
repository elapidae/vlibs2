#include "vtimer.h"

#include "impl_vposix/wrap_sys_timerfd.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vpoll/real_poll.h"

using namespace impl_vposix;
using namespace impl_vpoll;

static constexpr auto ns_ratio = std::chrono::nanoseconds::period::den;

//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class vtimer::_pimpl : epoll_receiver
{
public:
    _pimpl( vtimer *owner_ );
    ~_pimpl() override;

    void on_ready_read() override;

    vtimer *owner;
    int fd;
};
#pragma GCC diagnostic pop
//---------------------------------------------------------------------------------------
vtimer::_pimpl::_pimpl( vtimer * owner_ )
    : owner( owner_ )
    , fd( wrap_sys_timerfd::create() )
{
    real_poll::add_read( fd, this );
}
//---------------------------------------------------------------------------------------
vtimer::_pimpl::~_pimpl()
{
    real_poll::del( fd );
    wrap_unistd::close( fd );
}
//---------------------------------------------------------------------------------------
void vtimer::_pimpl::on_ready_read()
{
    auto count  = wrap_sys_timerfd::read( fd );
    owner->timeout();
    owner->timeout_times( count );
}
//=======================================================================================
vtimer::vtimer()
    : _p( new _pimpl(this) )
{}
//=======================================================================================
vtimer::~vtimer()
{}
//=======================================================================================
void vtimer::start( const std::chrono::nanoseconds& ns )
{
    auto sec     = ns.count() / ns_ratio;
    auto nanosec = ns.count() % ns_ratio;

    wrap_sys_timerfd::start( _p->fd, sec, nanosec );
}
//=======================================================================================
void vtimer::singleshot( const std::chrono::nanoseconds& ns )
{
    auto sec     = ns.count() / ns_ratio;
    auto nanosec = ns.count() % ns_ratio;

    wrap_sys_timerfd::singleshot( _p->fd, sec, nanosec );
}
//=======================================================================================
void vtimer::stop()
{
    wrap_sys_timerfd::stop( _p->fd );
}
//=======================================================================================
