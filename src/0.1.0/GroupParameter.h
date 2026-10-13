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

#ifndef RCP_V010_GROUPPARAMETER_H
#define RCP_V010_GROUPPARAMETER_H

#include "../GroupParameter.h"
#include "../Parameter.h"

#include "TypeDefinitionGroup.h"

namespace rcp {
namespace v010 {


class GroupParameter;
using GroupParameterPtr = std::shared_ptr<GroupParameter>;



class GroupParameter
    : public rcp::Parameter<TypeDefinitionGroup>
    , public rcp::GroupParameter
{
public:
    static GroupParameterPtr create(int16_t id)
    {
        return std::make_shared<GroupParameter>(id);
    }

public:
    GroupParameter(int16_t id, std::string label = "")
        : rcp::Parameter<TypeDefinitionGroup>(id, label)
        , rcp::IParameter(id, label)
    {}


public:
    // IGroupParameter
    IdType getId() const override
    {
        return Parameter<TypeDefinitionGroup>::getId();
    }

public:
    // IParameter
    bool isGroup() const override { return true; }


private:
    inline IGroupParameterPtr shared() override
    {
        return std::dynamic_pointer_cast<IGroupParameter>(std::enable_shared_from_this<Parameter<TypeDefinitionGroup>>::shared_from_this());
    }


};

}
}

#endif // GROUPPARAMETER_H
