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

#ifndef WRITER_H
#define WRITER_H

#include <inttypes.h>

#include "color.h"
#include "range.h"
#include "ip.h"
#include "tinystring.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"

namespace rcp {

class Writer
{
public:
    virtual ~Writer();

    virtual void write(const bool& c) = 0;
    virtual void write(const char& c) = 0;
    virtual void write(const uint8_t& c) = 0;
    virtual void write(const int8_t& c) = 0;
    virtual void write(const uint16_t& c) = 0;
    virtual void write(const int16_t& c) = 0;
    virtual void write(const uint32_t& c) = 0;
    virtual void write(const int32_t& c) = 0;
    virtual void write(const uint64_t& c) = 0;
    virtual void write(const int64_t& c) = 0;
    virtual void write(const float& c) = 0;
    virtual void write(const double& c) = 0;
    virtual void write(const std::string& s, bool prefix = true) = 0;
    virtual void write(const rcp::Color& s) = 0;
    virtual void write(const rcp::IPv4& s) = 0;
    virtual void write(const rcp::IPv6& s) = 0;
    virtual void write(const char* data, size_t length) = 0;
    virtual void write(std::vector<char> data) {
        write(data.data(), data.size());
    }

    template<typename T>
    void write(const Range<T>& c) {
        write(c.value1());
        write(c.value2());
    }

    template<typename T>
    void write(const Vector2<T>& c) {
        write(c.x());
        write(c.y());
    }

    template<typename T>
    void write(const Vector3<T>& c) {
        write(c.x());
        write(c.y());
        write(c.z());
    }

    template<typename T>
    void write(const Vector4<T>& c) {
        write(c.x());
        write(c.y());
        write(c.z());
        write(c.w());
    }

    void write(const TinyString& s) {
        writeTinyString(s);
    }

    void writeTinyString(const std::string& s) {
        if (s.length() >= UINT8_MAX) {
            write(UINT8_MAX);
            write(s.substr(0, UINT8_MAX), false); // write without length-prefix
        } else {
            write(static_cast<uint8_t>(s.length()));
            write(s, false); // write without length-prefix
        }
    }
    void writeShortString(const std::string& s) {
        if (s.length() >= UINT16_MAX) {
            write(UINT16_MAX);
            write(s.substr(0, UINT16_MAX), false); // write without length-prefix
        } else {
            write(static_cast<uint16_t>(s.length()));
            write(s, false); // write without length-prefix
        }
    }
    void writeString(const std::string& s) {
        if (s.length() >= UINT32_MAX) {
            write(UINT32_MAX);
            write(s.substr(0, UINT32_MAX), false); // write without length-prefix
        } else {
            write(static_cast<uint32_t>(s.length()));
            write(s, false); // write without length-prefix
        }
    }
};


template<typename T>
Writer& operator<<(Writer& out, const T& c)
{
    out.write(c);
    return out;
}

template<typename T>
Writer& operator<<(Writer& out, const Range<T>& c)
{
    out.write(c.value1());
    out.write(c.value2());
    return out;
}

}

#endif // WRITER_H
