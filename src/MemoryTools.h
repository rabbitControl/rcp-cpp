/*
********************************************************************
* RabbitControl - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of RabbitControl for c++.
*
* Written by Ingo Randolf, 2018-2026
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_MEMORYTOOLS_H
#define RCP_MEMORYTOOLS_H


// #define DO_MEMORY_TEST


#ifdef DO_MEMORY_TEST

#include <cstdint>

namespace rcp {

class MemoryTools
{
public:
    static int32_t counter;
    static int32_t upcounter;
    static int32_t downcounter;
    static void countUp();
    static void countDown();
    static void report();
};

}

#endif

#endif // RCP_MEMORYTOOLS_H
