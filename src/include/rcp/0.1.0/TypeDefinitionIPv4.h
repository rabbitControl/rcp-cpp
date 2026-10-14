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

#ifndef RCP_V010_TYPEDEFINITIONIPV4_H
#define RCP_V010_TYPEDEFINITIONIPV4_H

#include "../Ip.h"

#include "TypeDefinitionDefault.h"
#include "types.h"

namespace rcp {
namespace v010 {


class TypeDefinitionIPv4
    : public TypeDefinitionDefault<IPv4, v010::DATATYPE_IPV4>
{
public:
    TypeDefinitionIPv4(IParameter* param)
        : TypeDefinitionDefault<IPv4, v010::DATATYPE_IPV4>(param)
    {}

public:
    virtual IPv4 parseValue(std::istream& is) override
    {
        return readFromStream(is, IPv4{});
    }

    virtual void writeValue(Writer& out, const IPv4& value) override
    {
        out << value;
    }

};


}
}

#endif // TYPEDEFINITIONIPV4_H
