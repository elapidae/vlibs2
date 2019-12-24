#include "vtimer.h"

#include "impl_vposix/wrap_sys_timerfd.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/poll_context.h"
#include "impl_vposix/safe_fd.h"

using namespace impl_vposix;

static constexpr auto ns_ratio = std::chrono::nanoseconds::period::den;

//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
class vtimer::_pimpl : epoll_receiver
{
public:
    _pimpl( vtimer *owner_ );
    ~_pimpl() override;

    void on_events( epoll_receiver::events ) override;

    vtimer *owner;
    safe_fd fd;
};
#pragma GCC diagnostic pop
//---------------------------------------------------------------------------------------
vtimer::_pimpl::_pimpl( vtimer * owner_ )
    : owner( owner_ )
    , fd( wrap_sys_timerfd::create() )
{
    fd.poll_add_read( this );
}
//---------------------------------------------------------------------------------------
vtimer::_pimpl::~_pimpl()
{}
//---------------------------------------------------------------------------------------
void vtimer::_pimpl::on_events( epoll_receiver::events )
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
