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

#ifndef RCP_V10_CUSTOMPARAMETER_H
#define RCP_V10_CUSTOMPARAMETER_H

#include "TypeDefinitionCustom.h"
#include "ValueParameter.h"

namespace rcp {
namespace v10 {

class CustomParameter;
using CustomParameterPtr = std::shared_ptr<CustomParameter>;


class CustomParameter
    : public ValueParameter<std::vector<char>, TypeDefinitionCustom>
{
public:
    static CustomParameterPtr create(uint32_t id, uint32_t size)
    {
        return std::make_shared<CustomParameter>(id, size);
    }

public:
    CustomParameter(uint32_t id, uint32_t size)
        : ValueParameter<std::vector<char>, TypeDefinitionCustom>(id)
        , rcp::Parameter<TypeDefinitionCustom>(id)
        , rcp::IParameter(id)
    {
        if (id > RcpInt::MAX_VALUE)
        {
            throw std::runtime_error("invalid parameter id");
        }

        typeDefinition().setSize(size);
    }

};

using CustomParameterPtr = std::shared_ptr<CustomParameter>;

}
}


#endif // CUSTOMPARAMETER_H
