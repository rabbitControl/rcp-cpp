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

#ifndef RCP_VECTOR3_H
#define RCP_VECTOR3_H

#include <type_traits>
#include <ostream>

#include "StreamTools.h"
#include "TypeHelper.h"

namespace rcp {

template <class T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
class Vector3
{

public:
    Vector3()
    {}

    Vector3(T x, T y, T z)
    {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
    }

    Vector3(const Vector3& other)
    {
        std::memcpy(m_value, other.m_value, 3 * sizeof(T));
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

    void set(T x, T y, T z) {
        m_value[0] = x;
        m_value[1] = y;
        m_value[2] = z;
    }

    //
    bool operator==(const Vector3<T>& other) const {
        return std::memcmp(m_value, other.m_value, 3 * sizeof(T)) == 0;
    }

    bool operator!=(const Vector3<T>& other) const {
        return std::memcmp(m_value, other.m_value, 3 * sizeof(T)) != 0;
    }

    bool operator<(const Vector3<T>& other) const {
        return m_value[0] < other.m_value[0] &&
               m_value[1] < other.m_value[1] &&
               m_value[2] < other.m_value[2];
    }

    bool operator<=(const Vector3<T>& other) const {
        return m_value[0] <= other.m_value[0] &&
               m_value[1] <= other.m_value[1] &&
               m_value[2] <= other.m_value[2];
    }

    bool operator>(const Vector3<T>& other) const {
        return m_value[0] > other.m_value[0] &&
               m_value[1] > other.m_value[1] &&
               m_value[2] > other.m_value[2];
    }

    bool operator>=(const Vector3<T>& other) const {
        return m_value[0] >= other.m_value[0] &&
               m_value[1] >= other.m_value[1] &&
               m_value[2] >= other.m_value[2];
    }

    Vector3& operator=(const Vector3& other) {
        std::memcpy(m_value, other.m_value, 3 * sizeof(T));
        return *this;
    }

private:
    T m_value[3] = {0, 0, 0};
};


using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;


template <typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
Vector3<T>& swap_endian(Vector3<T>& u)
{
    u.setX(swap_endian(u.x()));
    u.setY(swap_endian(u.y()));
    u.setZ(swap_endian(u.z()));
    return u;
}

template <typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
Vector3<T> readFromStream(std::istream& is, const Vector3<T>& /*i*/)
{
    T x = readFromStream(is, x);
    T y = readFromStream(is, y);
    T z = readFromStream(is, z);

    return Vector3<T>(x, y, z);
}

template <typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
std::ostream& operator<<(std::ostream& out, const Vector3<T>& v)
{
    out << v.x();
    out << v.y();
    out << v.z();
    return out;
}



template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
Vector3<T> type_min(Vector3<T>)
{
    return Vector3<T>(std::numeric_limits<T>::min(),
                      std::numeric_limits<T>::min(),
                      std::numeric_limits<T>::min());
}

template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
Vector3<T> type_max(Vector3<T>)
{
    return Vector3<T>(std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max(),
                      std::numeric_limits<T>::max());
}

template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
Vector3<T> type_zero(Vector3<T>)
{
    return Vector3<T>();
}

template<typename T,
         typename = std::enable_if_t<is_scalar_element_v<T>> >
std::string value_to_string(Vector3<T> value)
{
    return std::to_string(value.x()) + "," +
           std::to_string(value.y()) + "," +
           std::to_string(value.z());
}

}

#endif // VECTOR3_H
