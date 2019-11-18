#include "vtime_point.h"

//=======================================================================================
namespace std
{
    //===================================================================================
    ostream& operator << ( ostream & os, const chrono::hours & val )
    {
        os << "hours(" << val.count() << ')';
        return os;
    }
    //===================================================================================
    ostream& operator << ( ostream & os, const chrono::minutes & val )
    {
        os << "minutes(" << val.count() << ')';
        return os;
    }
    //===================================================================================
    ostream& operator << ( ostream & os, const chrono::seconds & val )
    {
        os << "seconds(" << val.count() << ')';
        return os;
    }
    //===================================================================================
    ostream& operator << ( ostream & os, const chrono::milliseconds & val )
    {
        os << "milliseconds(" << val.count() << ')';
        return os;
    }
    //===================================================================================
    ostream& operator << ( ostream & os, const chrono::microseconds & val )
    {
        os << "microseconds(" << val.count() << ')';
        return os;
    }
    //===================================================================================
    ostream& operator << ( ostream & os, const chrono::nanoseconds & val )
    {
        os << "nanoseconds(" << val.count() << ')';
        return os;
    }
    //===================================================================================
} // namespace std
//=======================================================================================
