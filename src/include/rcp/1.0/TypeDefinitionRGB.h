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

#ifndef RCP_V10_TYPEDEFINITIONRGB_H
#define RCP_V10_TYPEDEFINITIONRGB_H

#include "../Color.h"

#include "types.h"
#include "TypeDefinitionDefault.h"

namespace rcp {
namespace v10 {


template<uint8_t type>
class TypeDefinitionColor
    : public TypeDefinitionDefault<Color, type>
{
public:
    TypeDefinitionColor(rcp::IParameter* param)
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


using TypeDefinitionRGB = TypeDefinitionColor<DATATYPE_RGB>;
using TypeDefinitionRGBA = TypeDefinitionColor<DATATYPE_RGBA>;


}
}


#endif // TYPEDEFINITIONRGB_H
