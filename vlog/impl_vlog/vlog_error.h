#ifndef IMPL_VLOG_VLOG_ERROR_H
#define IMPL_VLOG_VLOG_ERROR_H

#include <sstream>
#include "vlog.h"
#include "vcat.h"

//=======================================================================================
//      vlog::error
//=======================================================================================
class vlog::error final : public std::exception,
                          public impl_vcat::vcat_iface<vlog::error>
{
public:
    error( const char*  file,
           int32_t      line,
           const char*  func );

    error( const error& other );

    const char* what() const noexcept override;

private:
    vtime_point  _stamp;
    const char*  _file;
    int32_t      _line;
    const char*  _func;

    bool _sealed = false;
    std::string _sealed_msg {};
    std::ostringstream _stream;

    friend class impl_vcat::vcat_iface<vlog::error>;
    template<typename T>
    void do_cat( T && data );
};
//=======================================================================================
//      vlog::error
//=======================================================================================


//=======================================================================================
//      implementation
//=======================================================================================
#include <cassert>
template<typename T>
void vlog::error::do_cat( T && data )
{
    assert( !_sealed );
    if ( _sealed ) return;
    _stream << std::forward<T>(data);
}
//=======================================================================================

#endif // IMPL_VLOG_VLOG_ERROR_H
