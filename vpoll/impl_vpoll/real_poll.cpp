#include "real_poll.h"

using namespace impl_vpoll;
using namespace impl_vposix;

//=======================================================================================

static thread_local epoll local_poll;
static thread_local bool  local_let_stop = false;

//=======================================================================================
#include "vlog.h"
void impl_vpoll::real_poll::poll()
{
    vdeb << "poll in" << &local_poll << &local_let_stop;
    while ( !local_let_stop )
    {
        local_poll.wait_once();
    }
}
//=======================================================================================
void impl_vpoll::real_poll::stop()
{
    local_let_stop = true;
}
//=======================================================================================
void real_poll::add_read( int fd, epoll_receiver *receiver )
{
    local_poll.add_read( fd, receiver );
}
//=======================================================================================
void real_poll::add_write( int fd, epoll_receiver *receiver )
{
    local_poll.add_write( fd, receiver );
}
//=======================================================================================
void real_poll::add_rw( int fd, epoll_receiver *receiver )
{
    local_poll.add_rw( fd, receiver );
}
//=======================================================================================
void real_poll::mod_read( int fd, epoll_receiver *receiver )
{
    local_poll.mod_read( fd, receiver );
}
//=======================================================================================
void real_poll::mod_write( int fd, epoll_receiver *receiver )
{
    local_poll.mod_write( fd, receiver );
}
//=======================================================================================
void real_poll::mod_rw( int fd, epoll_receiver *receiver )
{
    local_poll.mod_rw( fd, receiver );
}
//=======================================================================================
void real_poll::del( int fd )
{
    local_poll.del( fd );
}
//=======================================================================================
