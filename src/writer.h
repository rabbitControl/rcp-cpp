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

#ifndef RCP_WRITER_H
#define RCP_WRITER_H


#include "Color.h"
#include "Range.h"
#include "Ip.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ColorFloat.h"

namespace rcp {

class Writer
{
public:
    virtual ~Writer()
    {}

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
    virtual void write(const char* data, size_t length) = 0;

    virtual void write(const rcp::Color& s) = 0;
    virtual void write(const rcp::IPv4& s) = 0;
    virtual void write(const rcp::IPv6& s) = 0;

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

    void write(const ColorFloatRGB& c) {
        write(c.r());
        write(c.g());
        write(c.b());
    }

    void write(const ColorFloatRGBA& c) {
        write(c.r());
        write(c.g());
        write(c.b());
        write(c.a());
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
