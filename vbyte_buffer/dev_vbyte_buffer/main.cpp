#include <iostream>

#include "vbyte_buffer.h"

using namespace std;

int main()
{
    cout << "is big: " << v::endian::is_big() << endl;
    cout << "is little: " << v::endian::is_little() << endl;

    char     center = 0x00;
    int32_t  i32    = 0x12345678;
    uint16_t u16    = 0xABCD;

    vbyte_buffer be, le;

    be.append_BE(i32);
    be.append(center);
    be.append_BE(u16);
    auto be_Hex = be.to_Hex();

    le.append_LE(i32);
    le.append_LE(center);
    le.append_LE(u16);
    auto le_Hex = le.to_Hex();

    auto test1 = be_Hex.str() == "12 34 56 78 00 AB CD";
    auto test2 = le_Hex.str() == "78 56 34 12 00 CD AB";

    cout << test1 << test2 << endl;
    cout << le_Hex << endl;
    if (!test1 || !test2) throw 42;
    cout << "OK" << endl;
}
