#include <iostream>

#include "vutf8_validator.h"

using namespace std;

int main()
{
    cout << vutf8_validator::valid( "Абракадабра!!!" ) << endl;

    if ( !vutf8_validator::valid( "Абракадабра!!!" ) ) return 1;

    string s = "ПРАЛРОМПАпардлоаоПРАРАЛГорпдорЗЖДРлдоплопрроеплошнрмпЛпр";
    cout << vutf8_validator::valid( s ) << endl;
    if ( !vutf8_validator::valid(s) ) return 1;

    cout << showbase << oct << uint((unsigned char)(s[17])) << endl;
    s[17] = 0320;
    int pos;
    cout << vutf8_validator::valid( s, &pos ) << endl;
    cout << dec << pos << endl;

    if ( pos != 17 ) return 1;

    return 0;
}
