#ifndef IMPL_VPOSIX_WRAP_ERRNO_H
#define IMPL_VPOSIX_WRAP_ERRNO_H

//=======================================================================================
/*
 *  2019-11-25  by elapidae
 *
 * http://man7.org/linux/man-pages/man3/errno.3.html
 *
 * Соответствия ошибок есть здесь:
 * https://ru.cppreference.com/w/cpp/error/errc
*/
//=======================================================================================

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class ErrNo final
    {
    public:
        ErrNo();

        bool has() const;
        std::string text() const;

        //  Бросает verror(text) если есть ошибка.
        void do_throw( const std::string& msg );

        //  EINTR -- Interrupted system call, надо повторить последний вызов.
        bool need_repeat_last_call() const;

        //  EAGAIN || EWOULDBLOCK;
        bool resource_unavailable_try_again() const;

    private:
        int _err;
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_WRAP_ERRNO_H
