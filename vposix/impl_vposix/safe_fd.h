#ifndef IMPL_VPOSIX_SAFE_FD_H
#define IMPL_VPOSIX_SAFE_FD_H

#include "impl_vposix/wrap_sys_epoll.h"

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    struct safe_fd final
    {
        //-------------------------------------------------------------------------------
        safe_fd() noexcept;
        safe_fd( int _fd ) noexcept;
        ~safe_fd();

        safe_fd( safe_fd&& ) noexcept;
        safe_fd& operator = ( safe_fd&& ) noexcept;

        safe_fd& operator = ( int other );

        //-------------------------------------------------------------------------------
        void close();

        operator int() const noexcept;
        bool has() const noexcept;

        bool in_poll() const noexcept;

        //-------------------------------------------------------------------------------
        void poll_add_read   ( epoll_receiver * receiver );
        void poll_add_write  ( epoll_receiver * receiver );
        void poll_add_rw     ( epoll_receiver * receiver );

        void poll_mod_read   ( epoll_receiver * receiver );
        void poll_mod_write  ( epoll_receiver * receiver );
        void poll_mod_rw     ( epoll_receiver * receiver );

        void poll_del();

        //-------------------------------------------------------------------------------
    private:
        bool _in_poll = false;
        int  _fd = -1;

        safe_fd( const safe_fd& ) = delete;
        safe_fd& operator = ( const safe_fd& ) = delete;
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_SAFE_FD_H
