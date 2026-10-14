#include "rcp/MemoryTools.h"

#ifdef DO_MEMORY_TEST

#include <iostream>

namespace rcp {

int32_t MemoryTools::counter = 0;
int32_t MemoryTools::upcounter = 0;
int32_t MemoryTools::downcounter = 0;

void MemoryTools::countUp()
{
    upcounter++;
    counter++;
}

void MemoryTools::countDown()
{
    downcounter++;
    counter--;
}

void MemoryTools::report()
{
    std::cout << "leaks: " << counter << " (↑" << upcounter << "/↓" << downcounter << ")" << std::endl;
}



}

#endif
