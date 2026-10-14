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

#ifndef RCP_V010_TYPEDEFINITIONS_H
#define RCP_V010_TYPEDEFINITIONS_H

#include "TypeDefinitionNumber.h"

namespace rcp {
namespace v010 {

// numbers
using TypeDefinitionInt8 =  TypeDefinitionNumber<int8_t>;
using TypeDefinitionUInt8 =  TypeDefinitionNumber<uint8_t>;
using TypeDefinitionInt16 =  TypeDefinitionNumber<int16_t>;
using TypeDefinitionUInt16 =  TypeDefinitionNumber<uint16_t>;
using TypeDefinitionInt32 =  TypeDefinitionNumber<int32_t>;
using TypeDefinitionUInt32 =  TypeDefinitionNumber<uint32_t>;
using TypeDefinitionInt64 =  TypeDefinitionNumber<int64_t>;
using TypeDefinitionUInt64 =  TypeDefinitionNumber<uint64_t>;

using TypeDefinitionFloat32 =  TypeDefinitionNumber<float>;
using TypeDefinitionFloat64 =  TypeDefinitionNumber<double>;

using TypeDefinitionVector2I32 =  TypeDefinitionNumber<Vector2<int32_t>>;
using TypeDefinitionVector2F32 =  TypeDefinitionNumber<Vector2<float>>;

using TypeDefinitionVector3I32 =  TypeDefinitionNumber<Vector3<int32_t>>;
using TypeDefinitionVector3F32 =  TypeDefinitionNumber<Vector3<float>>;

using TypeDefinitionVector4I32 =  TypeDefinitionNumber<Vector4<int32_t>>;
using TypeDefinitionVector4F32 =  TypeDefinitionNumber<Vector4<float>>;

}
}

#endif // TYPEDEFINITIONS_H
