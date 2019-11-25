#ifndef IMPL_VPOSIX_VERRNO_H
#define IMPL_VPOSIX_VERRNO_H

//=======================================================================================
/*
 *  2019-11-25  by elapidae
 *
 * http://man7.org/linux/man-pages/man3/errno.3.html
 *
 *  Введено такое имя, чтобы никак не совпасть с errno, errno.h etc.
*/
//=======================================================================================

#include <string>

//=======================================================================================
namespace impl_vposix
{
    //===================================================================================
    class VErrNo final
    {
    public:
        VErrNo();

        bool has() const;
        std::string text() const;

        //  Бросает std::runtime_error(text) если есть ошибка.
        void throw_if_has();

        //  EINTR -- Interrupted system call, надо повторить последний вызов.
        bool need_repeat_last_call() const;

    private:
        int _err;
    };
    //===================================================================================
} // namespace impl_vposix
//=======================================================================================

#endif // IMPL_VPOSIX_VERRNO_H
