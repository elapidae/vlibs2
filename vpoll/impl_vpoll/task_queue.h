#ifndef IMPL_VPOLL_TASK_QUEUE_H
#define IMPL_VPOLL_TASK_QUEUE_H

#include <queue>
#include <mutex>
#include "vinvoke_iface.h"

//=======================================================================================
namespace impl_vpoll
{
    //===================================================================================
    struct task_queue
    {
        static task_queue * current();

        void push( vinvoke_iface::func_invokable && f );

        vinvoke_iface::func_invokable pop();

    private:
        std::queue<vinvoke_iface::func_invokable> _queue;
        std::mutex _mutex;
    };
    //===================================================================================
}
//=======================================================================================

#endif // IMPL_VPOLL_TASK_QUEUE_H
