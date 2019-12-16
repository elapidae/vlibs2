#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include "vinvoke_iface.h"


//=======================================================================================
class vapplication final : public vinvoke_iface
{
public:
    size_t tasks_count() const override;
    void   tasks_clear() override;

    static void poll();
    static void stop();

private:
    void _invoke( task_type && f ) override;
};
//=======================================================================================


#endif // VAPPLICATION_H
