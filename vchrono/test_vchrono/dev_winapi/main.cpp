#include <iostream>
#include "vtime_point.h"


using namespace std;


int main()
{
    cout << vtime_point::now().humanable().iso8601_zzz() << endl;
}
