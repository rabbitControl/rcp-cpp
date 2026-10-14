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

#ifndef RCP_IGROUPPARAMETER_H
#define RCP_IGROUPPARAMETER_H

#include <memory>

#include "IParameter.h"


namespace rcp {

class IGroupParameter;
using IGroupParameterPtr = std::shared_ptr<IGroupParameter>;


class IGroupParameter
{
public:
    virtual void addChild(IParameterPtr) = 0;
    virtual void removeChild(IParameterPtr) = 0;
    virtual void clear() = 0;
    virtual std::map<IdType, IParameterPtr> children() const = 0;

    virtual IdType getId() const = 0;

    virtual void addChildInternal(IParameterPtr) = 0;
};


}

#endif // IGROUPPARAMETER_H
