/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2023
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
        Color() {}
        Color(uint32_t c) : value(c) {}

        uint32_t getValue() { return value; }
        uint32_t getValue() const { return value; }
        void setValue(uint32_t v) { value = v; }

        Color& operator=(const Color& other) {
            value = other.getValue();
            return *this;
        }

        bool operator==(const Color& other) {
            return value == other.getValue();
        }

        bool operator!=(const Color& other) {
            return value != other.getValue();
        }

    private:
        uint32_t value{0};
    };

    Color& swap_endian(const Color& u);
    Color readFromStream(std::istream& is, const Color& i);

    std::ostream& operator<<(std::ostream& out, const Color& v);
}

#endif
