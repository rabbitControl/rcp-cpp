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

#ifndef RCP_V10_TYPEDEFINITIONDEFAULT_H
#define RCP_V10_TYPEDEFINITIONDEFAULT_H

#include "../TypeDefinitionDefault.h"

#include "types.h"

namespace rcp {
namespace v10 {


template<typename T,
         uint8_t type>
class TypeDefinitionDefault
    : public rcp::TypeDefinitionDefault<T, type, OptionsDefault>
{
public:
    TypeDefinitionDefault(rcp::IParameter* param)
        : rcp::TypeDefinitionDefault<T, type, OptionsDefault>(param)
    {}

public:

    virtual void write(Writer& out, bool all) override
    {
        out.write(static_cast<char>(this->datatype() | ((all && this->m_defaultValue.hasValue()) || this->m_defaultValue.changed() ? 0x00 : Terminator)));


        if (this->m_defaultValue.hasValue())
        {
            if (all || this->m_defaultValue.changed())
            {
                out.write(static_cast<char>(OptionsDefault | Terminator));

                this->writeValue(out, this->m_defaultValue.value());

                if (!all)
                {
                    this->m_defaultValue.setUnchanged();
                }
            }
        }
        else if (this->m_defaultValue.changed())
        {
            out.write(static_cast<char>(OptionsDefault | Terminator));
            this->writeValue(out, type_zero(T{}));
            this->m_defaultValue.setUnchanged();
        }
    }

};


}
}

#endif // RCP_V10_TYPEDEFINITIONDEFAULT_H
