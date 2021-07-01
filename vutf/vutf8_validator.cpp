#include "vutf8_validator.h"

#include <cassert>

//=======================================================================================
bool vutf8_validator::valid( cstring test_str, int * err_pos ) noexcept
{
    int fake_pos;
    int& pos = err_pos ? *err_pos : fake_pos;

    vutf8_validator v;
    v.append( test_str );

    pos = v.position();
    return v.between();
}
//=======================================================================================
void vutf8_validator::append( cstring new_portion ) noexcept
{
    for ( auto c: new_portion )
        append( c );
}
//=======================================================================================
static bool is_suffix( unsigned char ch ) noexcept
{
    return ch >> 6 == 002;  // 10 xxxxxx(6)
}
//=======================================================================================
void vutf8_validator::append( unsigned char ch ) noexcept
{
    //  If error, use return, but OK stages use break, for increment position.

    switch ( stage )
    {
    //-----------------------------------------------------------------------------------
    case Error: return;
    //-----------------------------------------------------------------------------------
    case Between:

        if ( ch >> 7 == 0   )   {   break;                          }   //  ASCII symbol
        if ( ch >> 5 == 006 )   {   stage = Wait_1_Suffix; break;   }   //   110 xxxxx(5)
        if ( ch >> 4 == 016 )   {   stage = Wait_2_Suffix; break;   }   //  1 110 xxxx(4)
        if ( ch >> 3 == 036 )   {   stage = Wait_3_Suffix; break;   }   //  11 110 xxx(3)
                                {   stage = Error;         return;  }
    //-----------------------------------------------------------------------------------
    case Wait_3_Suffix: if ( is_suffix(ch) )    {   stage = Wait_2_Suffix; break;   }
                                                {   stage = Error;         return;  }

    case Wait_2_Suffix: if ( is_suffix(ch) )    {   stage = Wait_1_Suffix; break;   }
                                                {   stage = Error;         return;  }

    case Wait_1_Suffix: if ( is_suffix(ch) )    {   stage = Between;       break;   }
                                                {   stage = Error;         return;  }
    //-----------------------------------------------------------------------------------
    default: assert(false);
    //-----------------------------------------------------------------------------------
    } // switch ( stage )

    ++pos;
}
//=======================================================================================
bool vutf8_validator::error() const noexcept
{
    return stage == Error;
}
//=======================================================================================
bool vutf8_validator::between() const noexcept
{
    return stage == Between;
}
//=======================================================================================
int vutf8_validator::position() const noexcept
{
    return pos;
}
//=======================================================================================
