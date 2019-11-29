#include "vapplication.h"

#include <cassert>
#include <mutex>

#include "impl_vpoll/task_queue.h"
#include "impl_vpoll/real_poll.h"
#include "impl_vposix/wrap_sys_eventfd.h"

using namespace impl_vpoll;
using namespace impl_vposix;

//=======================================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wweak-vtables"
#pragma GCC diagnostic ignored "-Wpadded"
class _vapplication::_pimpl : public epoll_receiver
{
public:
    task_queue *app_queue = task_queue::current();
    eventfd event_fd;

    void on_ready_read() override
    {
        while ( event_fd.read() )
        {
            auto func = app_queue->pop();

            if ( !func )
                real_poll::stop();
            else
                func();
        }
    }
};
#pragma GCC diagnostic pop
//=======================================================================================
//      It is one instance of application.
//      Hack for init one copy.
_vapplication _init()
{
    return {};
}
_vapplication vapplication{ _init() };

//=======================================================================================
_vapplication::_vapplication()
    : _p( new _pimpl )
{
    real_poll::add_read( _p->event_fd.handle(), _p );
}
//=======================================================================================
_vapplication::~_vapplication()
{
    real_poll::del( _p->event_fd.handle() );
    delete _p;
}
//=======================================================================================
void _vapplication::stop()
{
    invoke( real_poll::stop );
}
//=======================================================================================
void _vapplication::_invoke( func_invokable && f )
{
    _p->app_queue->push( std::move(f) );
    _p->event_fd.write();
}
//=======================================================================================
