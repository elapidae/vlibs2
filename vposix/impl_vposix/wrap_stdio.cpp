#include "wrap_stdio.h"

#include <cstdio>
#include "impl_vposix/linux_call.h"

using namespace impl_vposix;

//=======================================================================================
void wrap_stdio::file_rename( const std::string& was, const std::string& will )
{
    linux_call::check( std::rename, was.c_str(), will.c_str() );
}
//=======================================================================================
