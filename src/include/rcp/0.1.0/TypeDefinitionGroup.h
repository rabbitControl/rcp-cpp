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

#ifndef RCP_V010_TYPEDEFINITIONGROUP_H
#define RCP_V010_TYPEDEFINITIONGROUP_H

#include "../TypeDefinition.h"

#include "types.h"

namespace rcp {
namespace v010 {


class TypeDefinitionGroup : public TypeDefinition<DATATYPE_GROUP>
{
public:
    TypeDefinitionGroup(IParameter* param) : rcp::TypeDefinition<v010::DATATYPE_GROUP>(param) {}

    void write(Writer& out, bool all) override
    {
        out.write(datatype());

        // no options

        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }
};


}
}

#endif // TYPEDEFINITIONGROUP_H
