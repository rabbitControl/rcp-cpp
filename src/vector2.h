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

#ifndef VECTOR2_H
#define VECTOR2_H

#include <type_traits>
#include <ostream>

#include "stream_tools.h"

namespace rcp {

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
class Vector2
{

public:
    Vector2()
    {}

    Vector2(T x, T y)
    {
        m_value[0] = x;
        m_value[1] = y;
    }

    Vector2(const Vector2& other)
    {
        std::memcpy(m_value, other.m_value, 2 * sizeof(T));
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

    void set(T x, T y) {
        m_value[0] = x;
        m_value[1] = y;
    }

    //
    bool operator==(const Vector2<T>& other) {
        return std::memcmp(m_value, other.m_value, 2 * sizeof(T)) == 0;
    }

    bool operator!=(const Vector2<T>& other) {
        return std::memcmp(m_value, other.m_value, 2 * sizeof(T)) != 0;
    }

    bool operator<(const Vector2<T>& other) {
        return m_value[0] < other.m_value[0] &&
               m_value[1] < other.m_value[1];
    }

    bool operator<=(const Vector2<T>& other) {
        return m_value[0] <= other.m_value[0] &&
               m_value[1] <= other.m_value[1];
    }

    bool operator>(const Vector2<T>& other) {
        return m_value[0] > other.m_value[0] &&
               m_value[1] > other.m_value[1];
    }

    bool operator>=(const Vector2<T>& other) {
        return m_value[0] >= other.m_value[0] &&
               m_value[1] >= other.m_value[1];
    }

    Vector2& operator=(const Vector2& other) {
        std::memcpy(m_value, other.m_value, 2 * sizeof(T));
        return *this;
    }

private:
    T m_value[2] = {0, 0};
};


typedef Vector2<float> Vector2f;
typedef Vector2<int> Vector2i;


template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
Vector2<T>& swap_endian(Vector2<T>& u)
{
    u.setX(swap_endian(u.x()));
    u.setY(swap_endian(u.y()));
    return u;
}

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
Vector2<T> readFromStream(std::istream& is, const Vector2<T>& /*i*/)
{
    T x = readFromStream(is, x);
    T y = readFromStream(is, y);

    return Vector2<T>(x, y);
}

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
std::ostream& operator<<(std::ostream& out, const Vector2<T>& v)
{
    out << v.x();
    out << v.y();
    return out;
}



template<typename T>
Vector2<T> type_min(Vector2<T>)
{
    return Vector2<T>(std::numeric_limits<T>::min(),
                      std::numeric_limits<T>::min());
}

template<typename T>
Vector2<T> type_max(Vector2<T>)
{
    return Vector2<T>(std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max());
}

template<typename T>
Vector2<T> type_zero(Vector2<T>)
{
    return Vector2<T>();
}

template<typename T>
std::string value_to_string(Vector2<T> value)
{
    return std::to_string(value.x()) + "," +
           std::to_string(value.y());
}


}


#endif // VECTOR2_H
