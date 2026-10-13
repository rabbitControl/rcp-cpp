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

#ifndef RCP_V010_TINYSTRING_H
#define RCP_V010_TINYSTRING_H

#include <string>

namespace rcp {
namespace v010 {

class TinyString
    : public std::string
{
public:
    TinyString() : std::string() {}
    TinyString(const char* str) : std::string(str) {}
    TinyString(const std::string value) : std::string(value) {}
};

} // namespace v010

std::string value_to_string(v010::TinyString value);


} // namespace rcp

#endif // TINYSTRING_H
