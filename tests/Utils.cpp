#include "Utils.h"

#include <iostream>

void Utils::log(std::istream &is)
{
    char ch;
    while(is.get(ch))
    {
        printf(" 0x%.2x", (ch & 0xFF));
    }
    std::cout << std::endl;
}
