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

#ifndef SPECIALIZETYPES_H
#define SPECIALIZETYPES_H

#include "types.h"
#include "color.h"
#include "ip.h"
#include "range.h"

enum td_types {
    td_default,
    td_string,
    td_num,
    td_enum,
    td_uri,
    td_array,
    td_custom
};


namespace rcp {

class Writeable;

// default
template <typename T>
struct isSpecialType
{ static const td_types value = std::is_arithmetic<T>::value ? td_num : td_default; };

template <>
struct isSpecialType<bool>
{ static const td_types value = td_default; };

template <>
struct isSpecialType<std::string>
{ static const td_types value = td_string; };

template <typename T>
struct isSpecialType<Range<T>>
{ static const td_types value = td_num; };


// convert datatype
template <typename T>
struct convertDatatype
{ static const datatype_t value = DATATYPE_CUSTOMTYPE; };

template <>
struct convertDatatype<bool>
{ static const datatype_t value = DATATYPE_BOOLEAN; };

template <>
struct convertDatatype<int8_t>
{ static const datatype_t value = DATATYPE_INT8; };
template <>
struct convertDatatype<uint8_t>
{ static const datatype_t value = DATATYPE_UINT8; };

template <>
struct convertDatatype<int16_t>
{ static const datatype_t value = DATATYPE_INT16; };
template <>
struct convertDatatype<uint16_t>
{ static const datatype_t value = DATATYPE_UINT16; };

template <>
struct convertDatatype<int32_t>
{ static const datatype_t value = DATATYPE_INT32; };
template <>
struct convertDatatype<uint32_t>
{ static const datatype_t value = DATATYPE_UINT32; };

template <>
struct convertDatatype<int64_t>
{ static const datatype_t value = DATATYPE_INT64; };
template <>
struct convertDatatype<uint64_t>
{ static const datatype_t value = DATATYPE_UINT64; };

template <>
struct convertDatatype<float>
{ static const datatype_t value = DATATYPE_FLOAT32; };
template <>
struct convertDatatype<double>
{ static const datatype_t value = DATATYPE_FLOAT64; };

template <>
struct convertDatatype<std::string>
{ static const datatype_t value = DATATYPE_STRING; };

}

#endif // SPECIALIZETYPES_H
