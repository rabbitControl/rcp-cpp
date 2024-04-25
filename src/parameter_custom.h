/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_PARAMETER_CUSTOM_H
#define RCP_PARAMETER_CUSTOM_H

#include "parameter_intern.h"

namespace rcp {

class CustomParameter;
typedef std::shared_ptr<CustomParameter> CustomParameterPtr;

class CustomParameter
    : public ValueParameter<std::vector<char>, CustomTypeDefinition, DATATYPE_CUSTOMTYPE>
{
public:
    static CustomParameterPtr create(int16_t id, uint32_t size)
    {
        return std::make_shared<CustomParameter>(id, size);
    }

public:
    CustomParameter(int16_t id, uint32_t size)
        : ValueParameter(id)
    {
        getDefaultTypeDefinition().setSize(size);
    }

    ~CustomParameter()
    {}

    // convenience
    void setDefault(const std::vector<char>& value)
    {
        // only allow same size
        if (value.size() == getRealTypeDef().getSize())
        {
            getDefaultTypeDefinition().setDefault(value);
        }
    }

    void setConfig(const std::vector<char>& config)
    {
        getDefaultTypeDefinition().setConfig(config);
    }

public:
    // ValueParameter
    bool setValue(const std::vector<char>& value) override
    {
        // only allow same size
        if (value.size() == getRealTypeDef().getSize())
        {
            return ValueParameter::setValue(value);
        }

        return false;
    }
};

}

#endif
