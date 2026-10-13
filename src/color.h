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

#ifndef RCP_COLOR_H
#define RCP_COLOR_H

#include <stdint.h>
#include <ostream>

namespace rcp {

class Color
{
public:
    Color()
    {}

    Color(uint32_t v)
        : value(v)
    {}

    Color(const Color& other)
        : value(other.value)
    {}

    uint32_t getValue() const { return value; }
    void setValue(uint32_t v) { value = v; }

    Color& operator=(const Color& other) {
        value = other.getValue();
        return *this;
    }

    Color& operator=(uint32_t v) {
        value = v;
        return *this;
    }

    bool operator==(const Color& other) const {
        return value == other.getValue();
    }

    bool operator!=(const Color& other) const {
        return value != other.getValue();
    }

private:
    uint32_t value{0};
};

Color& swap_endian(Color& u);
Color readFromStream(std::istream& is, const Color& i);
Color type_zero(const Color&);

std::ostream& operator<<(std::ostream& out, const Color& v);

std::string value_to_string(Color value);

}

#endif
