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

#ifndef RCP_V10_IPARAMETER_H
#define RCP_V10_IPARAMETER_H

#include "../IParameter.h"

namespace rcp {

class IParameter;
using IParameterPtr = std::shared_ptr<IParameter>;

namespace v10 {


class IParameter;
using IParameterPtr = std::shared_ptr<IParameter>;


class IParameter
    : public virtual rcp::IParameter

{
public:
    // readonly
    virtual bool getEnabled() const = 0;
    virtual void setEnabled(const bool enabled) = 0;
    virtual bool hasEnabled() const = 0;
    virtual void clearEnabled() = 0;


protected:
    friend class v10::ParameterParser;
    friend class v10::ParameterWriter;

    Option<bool> enabled{};
};



}
}


#endif // RCP_V10_IPARAMETER_H
