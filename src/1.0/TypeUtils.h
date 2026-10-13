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

#ifndef RCP_V10_TYPEUTILS_H
#define RCP_V10_TYPEUTILS_H

#include <cstdint>

#include "../Vector2.h"
#include "../Vector3.h"
#include "../Vector4.h"
#include "../Color.h"
#include "../Range.h"

#include "types.h"

namespace rcp {
namespace v10 {

// convert datatype
template <typename T>
struct convertDatatype
{ static const datatype_t value = DATATYPE_CUSTOMTYPE; };

// NOTE: for now used in numbers - see: TypeDefinitionNumber

// template <>
// struct convertDatatype<bool>
// { static const datatype_t value = DATATYPE_BOOLEAN; };

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
struct convertDatatype<Vector2<int32_t>>
{ static const datatype_t value = DATATYPE_VECTOR2I32; };

template <>
struct convertDatatype<Vector2<float>>
{ static const datatype_t value = DATATYPE_VECTOR2F32; };

template <>
struct convertDatatype<Vector3<int32_t>>
{ static const datatype_t value = DATATYPE_VECTOR3I32; };

template <>
struct convertDatatype<Vector3<float>>
{ static const datatype_t value = DATATYPE_VECTOR3F32; };

template <>
struct convertDatatype<Vector4<int32_t>>
{ static const datatype_t value = DATATYPE_VECTOR4I32; };

template <>
struct convertDatatype<Vector4<float>>
{ static const datatype_t value = DATATYPE_VECTOR4F32; };

// template <>
// struct convertDatatype<std::string>
// { static const datatype_t value = DATATYPE_STRING; };

// template <>
// struct convertDatatype<rcp::Color>
// { static const datatype_t value = DATATYPE_RGB; };

// template <typename T>
// struct convertDatatype<rcp::Range<T>>
// { static const datatype_t value = DATATYPE_RANGE; };




}
}

#endif // TYPEUTILS_H
