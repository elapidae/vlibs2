#include <iostream>

#include "vcompiler_traits.h"


using namespace std;

int main()
{
    cout << "Hello World! " << V_COMPILER_NAME_STR << " -> " << V_PLATFORM_NAME_STR
         << endl;

    cout << V_PRETTY_FUNCTION << endl;


    return 0;
}
