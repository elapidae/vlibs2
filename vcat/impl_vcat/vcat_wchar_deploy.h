#ifndef IMPL_VCAT_VCAT_WCHAR_DEPLOY_H
#define IMPL_VCAT_VCAT_WCHAR_DEPLOY_H

#include <ostream>

    std::ostream& operator << ( std::ostream& os, const wchar_t*      wstr );

//    std::ostream& operator << ( std::ostream& os, const std::wstring& wstr );
namespace std
{
    std::ostream& operator << ( std::ostream& os, const std::wstring& wstr );
}

#endif // IMPL_VCAT_VCAT_WCHAR_DEPLOY_H
