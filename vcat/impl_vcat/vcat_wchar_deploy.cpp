#include "vcat_wchar_deploy.h"

#include <codecvt>
#include <locale>


std::ostream& operator << ( std::ostream &os, const wchar_t* wstr )
{
    os << std::wstring( wstr );
    return os;
}

namespace std
{
    std::ostream& operator <<( std::ostream& os, const std::wstring& wstr )
    {
        std::wstring_convert<std::codecvt_utf8<wchar_t>> u8towide;
        os << u8towide.to_bytes( wstr );
        return os;
    }
}
