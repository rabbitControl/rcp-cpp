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

#include "rcp/ColorFloat.h"
#include "rcp/StreamTools.h"

namespace rcp {

ColorFloatRGB& swap_endian(ColorFloatRGB& u)
{
    u.setR(swap_endian(u.r()));
    u.setG(swap_endian(u.g()));
    u.setB(swap_endian(u.b()));
    return u;
}

ColorFloatRGB readFromStream(std::istream& is, const ColorFloatRGB& /*i*/)
{
    float r = readFromStream(is, float{});
    float g = readFromStream(is, float{});
    float b = readFromStream(is, float{});

    return ColorFloatRGB(r, g, b);
}

ColorFloatRGBA readFromStream(std::istream& is, const ColorFloatRGBA& /*i*/)
{
    float r = readFromStream(is, float{});
    float g = readFromStream(is, float{});
    float b = readFromStream(is, float{});
    float a = readFromStream(is, float{});

    return ColorFloatRGBA(r, g, b, a);
}

std::ostream& operator<<(std::ostream& out, const ColorFloatRGB& v)
{
    out << v.r() << v.g() << v.b();
    return out;
}

std::ostream& operator<<(std::ostream& out, const ColorFloatRGBA& v)
{
    out << v.r() << v.g() << v.b() << v.a();
    return out;
}

ColorFloatRGB type_zero(const ColorFloatRGB &)
{
    return {};
}

ColorFloatRGBA type_zero(const ColorFloatRGBA &)
{
    return {};
}

std::string value_to_string(const ColorFloatRGB& value)
{
    return std::to_string(value.r()) + "," +
           std::to_string(value.g()) + "," +
           std::to_string(value.b());
}

std::string value_to_string(const ColorFloatRGBA& value)
{
    return std::to_string(value.r()) + "," +
           std::to_string(value.g()) + "," +
           std::to_string(value.b()) + "," +
           std::to_string(value.a());
}

float ColorFloatRGB::r() const
{
    return m_r;
}

void ColorFloatRGB::setR(float newR)
{
    m_r = newR;
}

float ColorFloatRGB::g() const
{
    return m_g;
}

void ColorFloatRGB::setG(float newG)
{
    m_g = newG;
}

float ColorFloatRGB::b() const
{
    return m_b;
}

void ColorFloatRGB::setB(float newB)
{
    m_b = newB;
}

float ColorFloatRGBA::a() const
{
    return m_a;
}

void ColorFloatRGBA::setA(float newA)
{
    m_a = newA;
}

}
