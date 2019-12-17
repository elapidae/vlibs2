#include "impl_vposix/poll_context.h"

#include <signal.h>

using namespace impl_vposix;

//=======================================================================================
//  Устанавливает обработчик прерываний по Ctrl+C на остановку поллинга.
//=======================================================================================

//=======================================================================================
static void on_signal(int)
{
    poll_context::current()->stop();
}
//---------------------------------------------------------------------------------------
static int register_exit = []
{
    ::signal( SIGINT,  on_signal );
    ::signal( SIGKILL, on_signal );
    ::signal( SIGTERM, on_signal );

    //  Так не получится, возвращается pointer.
    //linux_call::check( ::signal, SIGINT,  &on_signal );
    //linux_call::check( ::signal, SIGKILL, &on_signal );
    //linux_call::check( ::signal, SIGTERM, &on_signal );

    return 0;
}();
//=======================================================================================
