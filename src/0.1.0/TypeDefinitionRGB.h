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

#ifndef RCP_V010_TYPEDEFINITIONRGB_H
#define RCP_V010_TYPEDEFINITIONRGB_H

#include "../Color.h"

#include "types.h"
#include "TypeDefinitionDefault.h"

namespace rcp {
namespace v010 {


template<uint8_t type>
class TypeDefinitionColor
    : public TypeDefinitionDefault<Color, type>
{
public:
    TypeDefinitionColor(IParameter* param)
        : TypeDefinitionDefault<Color, type>(param)
    {}

public:
    virtual Color parseValue(std::istream& is) override
    {
        return readFromStream(is, uint32_t{});
    }

    virtual void writeValue(Writer& out, const Color& value) override
    {
        out.write(value.getValue());
    }

};


using TypeDefinitionRGB = TypeDefinitionColor<v010::DATATYPE_RGB>;
using TypeDefinitionRGBA = TypeDefinitionColor<v010::DATATYPE_RGBA>;


}
}


#endif // TYPEDEFINITIONRGB_H
