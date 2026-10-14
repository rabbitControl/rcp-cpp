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

#include "rcp/StreamTools.h"

#include <sstream>
#include <iomanip>

#include "rcp/TypeDefinition.h"

namespace rcp {

//---------------------------------------------------
// swap string
std::string& swap_endian(std::string &u)
{
    return u;
}

//---------------------------------------------------
// read from stream
bool readFromStream(std::istream& is, bool const& /*i*/)
{
    bool value;
    is.read(reinterpret_cast<char*>(&value), sizeof(bool));
    return value;
}

std::string readFromStream(std::istream& /*is*/, std::string const& i)
{
    return i;
}


bool type_zero(bool)
{
    return false;
}

std::string type_zero(const std::string &)
{
    return empty_string;
}

std::vector<char> type_zero(const std::vector<char> &)
{
    return {};
}

std::vector<uint32_t> type_zero(const std::vector<uint32_t> &)
{
    return {};
}



// to string
std::string value_to_string(std::string value)
{
    return value;
}

std::string value_to_string(std::vector<char> value)
{
    std::stringstream ss;
    ss << std::hex;
    for (auto i : value)
    {
        ss << "0x" << std::setw(2) << std::setfill('0') << (0xff & (unsigned int)i) << " ";
    }
    return ss.str();
}

std::string value_to_string(bool value)
{
    return std::to_string(value);
}


}
