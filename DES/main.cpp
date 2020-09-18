#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "DES.h"
#include "DES.cpp"

using namespace std;

int main()
{
    DES des = DES();

    des.jiami();
    des.jiemi();
    return 0;
}
