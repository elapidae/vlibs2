#ifndef IMPL_VPOSIX_SAFE_FD_H
#define IMPL_VPOSIX_SAFE_FD_H

//=======================================================================================
/*  2019-12-24  by elapidae
 *
 *  safe_fd -- промежуточный по использованию класс, для использования, например, в
 *  сокетах, портах и т.д., чтобы было удобно хранить и поллить дескрипторы файлов.
*/
//=======================================================================================

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
        void poll_add_read  ( epoll_receiver * receiver );
        void poll_add_write ( epoll_receiver * receiver );
        void poll_add_rw    ( epoll_receiver * receiver );

        void poll_mod_read  ();
        void poll_mod_write ();
        void poll_mod_rw    ();

        void poll_del       ();

        //-------------------------------------------------------------------------------
    private:
        epoll_receiver * _receiver = nullptr;
        int  _fd = -1;

        safe_fd( const safe_fd& ) = delete;
        safe_fd& operator = ( const safe_fd& ) = delete;
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_SAFE_FD_H
