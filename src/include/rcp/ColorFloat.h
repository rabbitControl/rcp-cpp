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

#ifndef RCP_COLOR_FLOAT_H
#define RCP_COLOR_FLOAT_H

#include <ostream>

namespace rcp {

class ColorFloatRGB
{
public:
    ColorFloatRGB()
    {}

    ColorFloatRGB(float r, float g, float b)
        : m_r(r)
        , m_g(g)
        , m_b(b)
    {}

    ColorFloatRGB(const ColorFloatRGB& other)
        : m_r(other.m_r)
        , m_g(other.m_g)
        , m_b(other.m_b)
    {}

    ColorFloatRGB& operator=(const ColorFloatRGB& other) {
        m_r = other.m_r;
        m_g = other.m_g;
        m_b = other.m_b;
        return *this;
    }

    bool operator==(const ColorFloatRGB& other) const {
        return m_r == other.m_r &&
               m_g == other.m_g &&
               m_b == other.m_b;
    }

    bool operator!=(const ColorFloatRGB& other) const {
        return m_r != other.m_r ||
               m_g != other.m_g ||
               m_b != other.m_b;
    }

    float r() const;
    void setR(float newR);

    float g() const;
    void setG(float newG);

    float b() const;
    void setB(float newB);

protected:
    // friend class ColorFloatRGBA;

    float m_r{0};
    float m_g{0};
    float m_b{0};
};



class ColorFloatRGBA
    : public ColorFloatRGB
{
public:
    ColorFloatRGBA()
    {}

    ColorFloatRGBA(float r, float g, float b, float a)
        : ColorFloatRGB(r, g, b)
        , m_a(a)
    {}

    ColorFloatRGBA(const ColorFloatRGBA& other)
        : ColorFloatRGB(other.m_r, other.m_g, other.m_b)
        , m_a(other.m_a)
    {}

    ColorFloatRGBA& operator=(const ColorFloatRGBA& other) {
        m_r = other.m_r;
        m_g = other.m_g;
        m_b = other.m_b;
        m_a = other.m_a;
        return *this;
    }

    bool operator==(const ColorFloatRGBA& other) const {
        return m_r == other.m_r &&
               m_g == other.m_g &&
               m_b == other.m_b &&
               m_a == other.m_a;
    }

    bool operator!=(const ColorFloatRGBA& other) const {
        return m_r != other.m_r ||
               m_g != other.m_g ||
               m_b != other.m_b ||
               m_a != other.m_a;
    }

    float a() const;
    void setA(float newA);

protected:
    float m_a{0};
};


ColorFloatRGB& swap_endian(ColorFloatRGB& u);

ColorFloatRGB readFromStream(std::istream& is, const ColorFloatRGB& i);
ColorFloatRGBA readFromStream(std::istream& is, const ColorFloatRGBA& i);

ColorFloatRGB type_zero(const ColorFloatRGB&);
ColorFloatRGBA type_zero(const ColorFloatRGBA&);

std::ostream& operator<<(std::ostream& out, const ColorFloatRGB& v);
std::ostream& operator<<(std::ostream& out, const ColorFloatRGBA& v);

std::string value_to_string(const ColorFloatRGB& value);
std::string value_to_string(const ColorFloatRGBA& value);

}

#endif
