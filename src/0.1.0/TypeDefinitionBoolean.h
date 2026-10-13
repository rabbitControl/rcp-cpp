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

#ifndef RCP_V010_BOOLEANTYPEDEFINITION_H
#define RCP_V010_BOOLEANTYPEDEFINITION_H

#include "types.h"
#include "TypeDefinitionDefault.h"

namespace rcp {
namespace v010 {


class TypeDefinitionBoolean
    : public TypeDefinitionDefault<bool, v010::DATATYPE_BOOLEAN>
{
public:
    TypeDefinitionBoolean(IParameter* param)
        : TypeDefinitionDefault<bool, v010::DATATYPE_BOOLEAN>(param)
    {}

    virtual bool parseValue(std::istream& is) override
    {
        return readFromStream(is, true);
    }

    virtual void writeValue(Writer& out, const bool& value) override
    {
        out.write(value);
    }

};

}
}


#endif // BOOLEANTYPEDEFINITION_H
