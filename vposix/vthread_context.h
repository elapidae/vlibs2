#ifndef VTHREAD_CONTEXT_H
#define VTHREAD_CONTEXT_H

//=======================================================================================
/*  vthread_context     by elapidae 2020-01-20
 *
 *  Предназначен для захвата текущего контекста потока, как главного так и любого
 *  другого. С помощью этого контекста можно передавать задачи в поток где был создан
 *  экземпляр vthread_context.
 *
 *
*/
//=======================================================================================

#include "vinvoke_iface.h"

//=======================================================================================
class vthread_context final : public vinvoke_iface
{
public:
    vthread_context();

    size_t tasks_count() const override;
    void   tasks_clear() override;

private:
    virtual void _invoke( task_type&& ) override;

private:
    void* _my_ctx;
};
//=======================================================================================

#endif // VTHREAD_CONTEXT_H
