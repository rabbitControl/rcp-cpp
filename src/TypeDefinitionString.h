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

#ifndef RCP_TYPEDEFINITIONSTRING_H
#define RCP_TYPEDEFINITIONSTRING_H

#include <string>

#include "TypeDefinitionDefault.h"
#include "Option.h"

namespace rcp
{

template<uint8_t type,
         uint8_t defaultprefix>
class TypeDefinitionString
    : public TypeDefinitionDefault<std::string, type, defaultprefix>
{
public:
    TypeDefinitionString(IParameter* param)
        : TypeDefinitionDefault<std::string, type, defaultprefix>(param)
    {}

    std::string getRegex() const {
        if (m_regex.hasValue())
        {
            return m_regex.value();
        }

        return "";
    }

    void setRegex(const std::string& r) {
        this->updateOption(m_regex, r);
    }

    bool hasRegex() const {
        return m_regex.hasValue();
    }

    void clearRegex() {
        this->clearOption(m_regex);
    }


    virtual void update(const TypeDefinitionString& other)
    {
        TypeDefinitionDefault<std::string, type, defaultprefix>::update(other);

        m_regex = other.m_regex;
    }

protected:
    Option<std::string> m_regex;
};

}


#endif // TYPEDEFINITIONSTRING_H
