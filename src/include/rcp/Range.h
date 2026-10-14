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

#ifndef RCP_RANGE_H
#define RCP_RANGE_H

#include <string>
#include <type_traits>

#include "TypeHelper.h"

namespace rcp {

template <class T,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<T>> >
class Range
{
public:
    Range()
    {}

    Range(T v1, T v2)
        : m_value1(v1)
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


    bool operator==(const Range<T>& other) const {
        return m_value1 == other.m_value1 && m_value2 == other.m_value2;
    }

    bool operator!=(const Range<T>& other) const {
        return m_value1 != other.m_value1 ||
               m_value2 != other.m_value2;
    }

    Range& operator=(const Range& other) {
        m_value1 = other.m_value1;
        m_value2 = other.m_value2;
        return *this;
    }

private:
    T m_value1;
    T m_value2;
};


template <typename T,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<T>> >
Range<T>& swap_endian(Range<T>& u) {
    u.setValue1(swap_endian(u.value1()));
    u.setValue2(swap_endian(u.value2()));
    return u;
}

template <typename T,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<T>> >
Range<T> readFromStream(std::istream& is, const Range<T>& /*i*/)
{
    T v1 = readFromStream(is, v1);
    T v2 = readFromStream(is, v2);

    return Range<T>(v1, v2);
}


template <typename T,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<T>> >
std::ostream& operator<<(std::ostream& out, const Range<T>& v) {
    out << std::to_string(v.value1());
    out << std::to_string(v.value2());
    return out;
}

template<typename T,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<T>> >
std::string value_to_string(Range<T> value)
{
    return std::to_string(value.value1()) + "," + std::to_string(value.value2());
}

template<typename T,
         typename = std::enable_if_t<is_valid_parameter_number_element_v<T>> >
Range<T> type_zero(Range<T>)
{
    return Range<T>();
}

}

#endif // RCP_RANGE_H
