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

#ifndef RCP_V10_TYPEDEFINITIONRGBFLOAT_H
#define RCP_V10_TYPEDEFINITIONRGBFLOAT_H

#include "../ColorFloat.h"

#include "types.h"
#include "TypeDefinitionDefault.h"

namespace rcp {
namespace v10 {


template<typename T,
         uint8_t type>
class TypeDefinitionColorFloat
    : public TypeDefinitionDefault<T, type>
{
public:
    TypeDefinitionColorFloat(rcp::IParameter* param)
        : TypeDefinitionDefault<T, type>(param)
    {}

public:
    virtual T parseValue(std::istream& is) override
    {
        return readFromStream(is, T{});
    }

    virtual void writeValue(Writer& out, const T& value) override
    {
        out << value;
    }

};


using TypeDefinitionRGBFloat = TypeDefinitionColorFloat<ColorFloatRGB, DATATYPE_RGB_FLOAT>;
using TypeDefinitionRGBAFloat = TypeDefinitionColorFloat<ColorFloatRGBA, DATATYPE_RGBA_FLOAT>;


}
}

#endif // TYPEDEFINITIONRGBFLOAT_H
