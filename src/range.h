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

#ifndef RCP_RANGE_H
#define RCP_RANGE_H

#include <limits>
#include <string>
#include <type_traits>

namespace rcp {

template <class T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
class Range
{
public:
    Range() :
        m_value1(0)
        , m_value2(0)
    {}

    Range(T v1, T v2) :
        m_value1(v1)
        , m_value2(v2)
    {}

    void set(T v1, T v2) {
        m_value1 = v1;
        m_value2 = v2;
    }

    void setValue1(T v) {
        m_value1 = v;
    }

    void setValue2(T v) {
        m_value2 = v;
    }

    T value1() const {
        return m_value1;
    }

    T value2() const {
        return m_value2;
    }


    bool operator==(const Range<T>& other) {
        return m_value1 == other.m_value1 && m_value2 == other.m_value2;
    }

    bool operator!=(const Range<T>& other) {
        return m_value1 != other.m_value1 ||
               m_value2 != other.m_value2;
    }

    Range& operator=(const Range& other) {
        m_value1 = other.m_value1;
        m_value2 = other.m_value2;
        return *this;
    }

private:
    T m_value1{0};
    T m_value2{0};
};



template <typename T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
Range<T> readFromStream(std::istream& is, const Range<T>& /*i*/)
{
    T v1 = readFromStream(is, v1);
    T v2 = readFromStream(is, v2);

    return Range<T>(v1, v2);
}


template <typename T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
std::ostream& operator<<(std::ostream& out, const Range<T>& v) {
    out << v.value1();
    out << v.value2();
    return out;
}

template<typename T,
         typename = std::enable_if<std::is_arithmetic<T>::value && !std::is_same<T, bool>::value > >
std::string value_to_string(Range<T> value)
{
    return std::to_string(value.value1()) + "," + std::to_string(value.value2());
}

}

#endif // RCP_RANGE_H
