#ifndef IMPL_VPOSIX_PIPE_H
#define IMPL_VPOSIX_PIPE_H

#include <string>

#include "vsignal.h"
#include "impl_vposix/wrap_unistd.h"
#include "impl_vposix/wrap_sys_epoll.h"

//=======================================================================================
namespace impl_vposix
{
    class pipe final : epoll_receiver
    {
    public:

        vsignal<std::string> has_read;
        void poll_read();

        vsignal<> read_has_closed;

        pipe();
        ~pipe() override;

        void close_read();
        void close_write();

        void dup_read ( int h );
        void dup_write( int h );

        std::string do_read();
        void write( const std::string& data );

    private:
        safe_fd _read;
        safe_fd _write;

        void on_events(events) override;
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_PIPE_H
