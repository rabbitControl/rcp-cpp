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

#ifndef RCP_V010_CUSTOMPARAMETER_H
#define RCP_V010_CUSTOMPARAMETER_H

#include "TypeDefinitionCustom.h"
#include "ValueParameter.h"

namespace rcp {
namespace v010 {

class CustomParameter;
using CustomParameterPtr = std::shared_ptr<CustomParameter>;

class CustomParameter
    : public ValueParameter<std::vector<char>, TypeDefinitionCustom>
{
public:
    static CustomParameterPtr create(int16_t id, uint32_t size)
    {
        return std::make_shared<CustomParameter>(id, size);
    }

public:
    CustomParameter(int16_t id, uint32_t size)
        : ValueParameter(id)
        , rcp::IParameter(id)
    {
        typeDefinition().setSize(size);
    }

};

}
}


#endif // CUSTOMPARAMETER_H
