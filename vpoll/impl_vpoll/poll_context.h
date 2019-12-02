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

        poll_context();
        ~poll_context();

        void push( task_type && task );

        void poll();
        void stop();

    private:
        class pimpl;
        std::unique_ptr<pimpl> p;
    };
    //===================================================================================
} // namespace impl_vpoll
//=======================================================================================

#endif // IMPL_VPOLL_POLL_CONTEXT_H
