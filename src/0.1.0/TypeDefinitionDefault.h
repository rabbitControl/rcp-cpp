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

#ifndef RCP_V010_TYPEDEFINITIONDEFAULT_H
#define RCP_V010_TYPEDEFINITIONDEFAULT_H

#include "../TypeDefinitionDefault.h"

#include "types.h"

namespace rcp {
namespace v010 {

template<typename T,
         uint8_t type>
class TypeDefinitionDefault
    : public rcp::TypeDefinitionDefault<T, type, v010::OptionsDefault>
{
public:
    TypeDefinitionDefault(IParameter* param)
        : rcp::TypeDefinitionDefault<T, type, v010::OptionsDefault>(param)
    {}

public:

    virtual void write(Writer& out, bool all) override
    {
        out.write(this->datatype());


        if (this->m_defaultValue.hasValue())
        {
            if (all || this->m_defaultValue.changed())
            {
                out.write(static_cast<char>(OPTIONS_DEFAULT));

                this->writeValue(out, this->m_defaultValue.value());

                if (!all)
                {
                    this->m_defaultValue.setUnchanged();
                }
            }
        }
        else if (this->m_defaultValue.changed())
        {
            out.write(static_cast<char>(v010::OptionsDefault));
            this->writeValue(out, type_zero(T{}));
            this->m_defaultValue.setUnchanged();
        }


        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }

};

}
}

#endif // RCP_V010_TYPEDEFINITIONDEFAULT_H
