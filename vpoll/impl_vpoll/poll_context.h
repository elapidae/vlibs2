#ifndef IMPL_VPOLL_POLL_CONTEXT_H
#define IMPL_VPOLL_POLL_CONTEXT_H

#include <memory>
#include "vinvoke_iface.h"

//=======================================================================================
namespace impl_vpoll
{
    //===================================================================================
    class poll_context final
    {
    public:
        using task_type = vinvoke_iface::func_invokable;

        static void poll();     //  Только из текущего потока.
        void stop();            //  Из любого потока.

        poll_context();

        void push( task_type && task );

        class thread_local_pimpl;
    private:
        thread_local_pimpl * p;
    };
    //===================================================================================
} // namespace impl_vpoll
//=======================================================================================

#endif // IMPL_VPOLL_POLL_CONTEXT_H
