#ifndef IMPL_VLOG_POSITION_FIX_H
#define IMPL_VLOG_POSITION_FIX_H

#include "vtime_point.h"

//=======================================================================================
namespace impl_vlog
{
    //===================================================================================
    class position_fix final
    {
    public:
        position_fix( const char* file, int line, const char* func );

        const vtime_point&  stamp()         const;
        const char*         filepath()      const;      // Full source path.
        std::string         filename()      const;      // File name only.
        int                 line()          const;
        const char*         function()      const;


        std::string         place()         const;      // [basefname:123]

        ~position_fix()                                 = default;
        position_fix( position_fix&& )                  = default;
        position_fix( const position_fix& )             = default;
        position_fix& operator= ( position_fix&& )      = default;
        position_fix& operator= ( const position_fix& ) = default;

    private:
        vtime_point _stamp;
        const char* _file;
        int         _line;
        const char* _func;

    }; // position_fix
    //===================================================================================
} // namespace impl_vlog
//=======================================================================================
#ifndef _MSC_VER
#define V_POSITION_FIX impl_vlog::position_fix( __FILE__, __LINE__, __PRETTY_FUNCTION__ )
#else
#define V_POSITION_FIX impl_vlog::position_fix( __FILE__, __LINE__, __func__ )
#endif
//=======================================================================================

#endif // IMPL_VLOG_POSITION_FIX_H
