#ifndef VAPPLICATION_H
#define VAPPLICATION_H

#include "vinvoke_iface.h"


//=======================================================================================
class vapplication final : public vinvoke_iface
{
public:

    static void poll();

    static void stop();

private:
    void _invoke( func_invokable && f ) override;
};
//=======================================================================================


#endif // VAPPLICATION_H
