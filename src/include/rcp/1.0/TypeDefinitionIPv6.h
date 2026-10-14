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

#ifndef RCP_V10_TYPEDEFINITIONIPV6_H
#define RCP_V10_TYPEDEFINITIONIPV6_H

#include "../Ip.h"

#include "TypeDefinitionDefault.h"
#include "types.h"

namespace rcp {
namespace v10 {


class TypeDefinitionIPv6
    : public TypeDefinitionDefault<IPv6, DATATYPE_IPV6>
{
public:
    TypeDefinitionIPv6(rcp::IParameter* param)
        : TypeDefinitionDefault<IPv6, DATATYPE_IPV6>(param)
    {}

public:
    virtual IPv6 parseValue(std::istream& is) override
    {
        return readFromStream(is, IPv6{});
    }

    virtual void writeValue(Writer& out, const IPv6& value) override
    {
        out.write(value);
    }

};


}
}


#endif // TYPEDEFINITIONIPV6_H
