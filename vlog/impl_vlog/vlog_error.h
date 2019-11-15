#ifndef IMPL_VLOG_VLOG_ERROR_H
#define IMPL_VLOG_VLOG_ERROR_H

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
    std::string _msg;

    friend class impl_vcat::vcat_iface<vlog::error>;
    template<typename T>
    void do_cat( T && data )
    { _msg.append( vcat(std::forward<T>(data)) ); }
};
//=======================================================================================
//      vlog::error
//=======================================================================================


#endif // IMPL_VLOG_VLOG_ERROR_H
