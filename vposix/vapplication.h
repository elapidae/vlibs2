#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include <vector>
#include <string>
#include "vinvoke_iface.h"

//=======================================================================================
/*
 * TODO: Сделать полноценный pid-locker, чтобы захватывать атомарно.
 *
 * TODO: Попробовать изменить имя vapplication на vmain_thread,
 * т.к. главная задача -- поллить главный поток.
*/
//=======================================================================================


//=======================================================================================
class vapplication final : public vinvoke_iface
{
public:
    size_t tasks_count() const override;
    void   tasks_clear() override;

    static void poll();
    static void stop();

    static pid_t pid() noexcept;

private:
    void _invoke( task_type && f ) override;
};
//=======================================================================================

#endif // VAPPLICATION_H
