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

#include "color.h"
#include "stream_tools.h"

namespace rcp {

Color& swap_endian(Color& u)
{
    u.setValue(swap_endian(u.getValue()));
    return u;
}

Color readFromStream(std::istream& is, const Color& /*i*/)
{
    uint32_t value;
    is.read(reinterpret_cast<char *>(&value), sizeof(uint32_t));

#if BYTE_ORDER == LITTLE_ENDIAN
    value = swap_endian(value);
#endif

    return Color(value);
}

std::ostream& operator<<(std::ostream& out, const Color& v)
{
    out << v.getValue();
    return out;
}

std::string value_to_string(Color value)
{
    return std::to_string(value.getValue());
}

}
