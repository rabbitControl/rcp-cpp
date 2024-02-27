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

#ifndef VECTOR4_H
#define VECTOR4_H


#include <type_traits>
#include <ostream>

#include "stream_tools.h"

namespace rcp {

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
class Vector4
{

public:
    Vector4()
    {}

    Vector4(T x, T y, T z, T w)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
        m_value[3] = w;
    }

    Vector4(const Vector4& other)
    {
        std::memcpy(m_value, other.m_value, 4 * sizeof(T));
    }

    // x
    void setX(T x)
    {
        m_value[0] = x;
    }

    T x() const {
        return m_value[0];
    }

    // y
    void setY(T y)
    {
        m_value[1] = y;
    }

    T y() const {
        return m_value[1];
    }

    // z
    void setZ(T z)
    {
        m_value[2] = z;
    }

    T z() const {
        return m_value[2];
    }

    // w
    void setW(T w)
    {
        m_value[3] = w;
    }

    T w() const {
        return m_value[3];
    }

    void set(T x, T y, T z, T w) {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
        m_value[3] = w;
    }

    //
    bool operator==(const Vector4<T>& other) {
        return std::memcmp(m_value, other.m_value, 4 * sizeof(T)) == 0;
    }

    bool operator!=(const Vector4<T>& other) {
        return std::memcmp(m_value, other.m_value, 4 * sizeof(T)) != 0;
    }

    Vector4& operator=(const Vector4& other) {
        std::memcpy(m_value, other.m_value, 4 * sizeof(T));
        return *this;
    }

private:
    T m_value[4] = {0, 0, 0, 0};
};


typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;


template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
Vector4<T>& swap_endian(Vector4<T>& u)
{
    u.setX(swap_endian(u.x()));
    u.setY(swap_endian(u.y()));
    u.setZ(swap_endian(u.z()));
    u.setW(swap_endian(u.w()));
    return u;
}

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
Vector4<T> readFromStream(std::istream& is, const Vector4<T>& /*i*/)
{
    T x = readFromStream(is, x);
    T y = readFromStream(is, y);
    T z = readFromStream(is, z);
    T w = readFromStream(is, w);

    return Vector4<T>(x, y, z, w);
}

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
std::ostream& operator<<(std::ostream& out, const Vector4<T>& v)
{
    out << v.x();
    out << v.y();
    out << v.z();
    out << v.w();
    return out;
}



template<typename T>
Vector4<T> type_min(Vector4<T>)
{
    return Vector4<T>(std::numeric_limits<T>::min(),
                      std::numeric_limits<T>::min(),
                      std::numeric_limits<T>::min(),
                      std::numeric_limits<T>::min());
}

template<typename T>
Vector4<T> type_max(Vector4<T>)
{
    return Vector4<T>(std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max());
}

template<typename T>
Vector4<T> type_zero(Vector4<T>)
{
    return Vector4<T>();
}

template<typename T>
std::string value_to_string(Vector4<T> value)
{
    return std::to_string(value.x()) + "," +
           std::to_string(value.y()) + "," +
           std::to_string(value.z()) + "," +
           std::to_string(value.w());
}

}

#endif // VECTOR4_H
