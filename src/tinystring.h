/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/
#ifndef TINYSTRING_H
#define TINYSTRING_H

#include <string>

namespace rcp {

class TinyString
    : public std::string
{
public:
    TinyString() : std::string() {}
    TinyString(const char* str) : std::string(str) {}
    TinyString(const std::string value) : std::string(value) {}
};

}

#endif // TINYSTRING_H
