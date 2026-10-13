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

#ifndef RCP_V10_PARAMETERDEFINITIONS_H
#define RCP_V10_PARAMETERDEFINITIONS_H

#include "TypeDefinitions.h"
#include "TypeDefinitionBoolean.h"
#include "TypeDefinitionString.h"
#include "TypeDefinitionRange.h"
#include "TypeDefinitionIPv4.h"
#include "TypeDefinitionIPv6.h"
#include "TypeDefinitionURI.h"
#include "TypeDefinitionRGB.h"
#include "TypeDefinitionEnum.h"
#include "TypeDefinitionRGBFloat.h"

#include "ValueParameter.h"
#include "BangParameter.h"
#include "CustomParameter.h"
#include "GroupParameter.h"


namespace rcp {
namespace v10 {

// using BangParameter = BangParameter<v10::TypeDefinitionBang>;
// using BangParameterPtr = std::shared_ptr<BangParameter>;

// using GroupParameter = rcp::GroupParameter<TypeDefinitionGroup>;
// using GroupParameterPtr = std::shared_ptr<GroupParameter>;

VALUEPARAMDEF(Boolean, bool);
VALUEPARAMDEF(String, std::string);
VALUEPARAMDEF(Enum, std::vector<uint32_t>)
VALUEPARAMDEF(URI, std::string)
VALUEPARAMDEF(RGB, Color)
VALUEPARAMDEF(RGBA, Color)
VALUEPARAMDEF(RGBFloat, ColorFloatRGB)
VALUEPARAMDEF(RGBAFloat, ColorFloatRGBA)
VALUEPARAMDEF(IPv4, IPv4)
VALUEPARAMDEF(IPv6, IPv6)

// Image
// List
// Array


// numbers
VALUEPARAMDEF(Int8, int8_t)
VALUEPARAMDEF(UInt8, uint8_t)
VALUEPARAMDEF(Int16, int16_t)
VALUEPARAMDEF(UInt16, uint16_t)
VALUEPARAMDEF(Int32, int32_t)
VALUEPARAMDEF(UInt32, uint32_t)
VALUEPARAMDEF(Int64, int64_t)
VALUEPARAMDEF(UInt64, uint64_t)

VALUEPARAMDEF(Float32, float)
VALUEPARAMDEF(Float64, double)

VALUEPARAMDEF(Vector2I32, Vector2<int32_t>)
VALUEPARAMDEF(Vector2F32, Vector2<float>)

VALUEPARAMDEF(Vector3I32, Vector3<int32_t>)
VALUEPARAMDEF(Vector3F32, Vector3<float>)

VALUEPARAMDEF(Vector4I32, Vector4<int32_t>)
VALUEPARAMDEF(Vector4F32, Vector4<float>)


template<typename T>
using RangeParameter = ValueParameter<Range<T>, TypeDefinitionRange<T>>;

template<typename T>
using RangeParameterPtr = std::shared_ptr<RangeParameter<T>>;

}
}


#endif // PARAMETERDEFINITIONS_H
