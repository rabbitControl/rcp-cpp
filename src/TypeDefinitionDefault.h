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

#ifndef RCP_TYPEDEFINITIONDEFAULT_H
#define RCP_TYPEDEFINITIONDEFAULT_H

#include <istream>

#include "Option.h"
#include "TypeDefinition.h"

namespace rcp
{

template<typename T,
         uint8_t type,
         uint8_t defaultprefix>
class TypeDefinitionDefault
    : public TypeDefinition<type>
{

public:

    T getDefaultValue() const
    {
        if (m_defaultValue.hasValue())
        {
            return m_defaultValue.value();
        }

        // ?
        return type_zero(T{});
    }

    void setDefaultValue(T defaultValue)
    {
        this->updateOption(m_defaultValue, defaultValue);
    }

    bool hasDefaultValue() const
    {
        return m_defaultValue.hasValue();
    }

    void clearDefault()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        this->clearOption(m_defaultValue);
    }



    virtual void update(const TypeDefinitionDefault& other)
    {
        TypeDefinition<type>::update(other);

        m_defaultValue = other.m_defaultValue;
    }

    virtual void setAllUnchanged() override
    {
        TypeDefinition<type>::setAllUnchanged();

        m_defaultValue.setUnchanged();
    }


public:
    // TODO: protect this?
    virtual T parseValue(std::istream& is) = 0;
    virtual void writeValue(Writer& out, const T& value) = 0;


    TypeDefinitionDefault(IParameter* param) : TypeDefinition<type>(param) {}
    TypeDefinitionDefault(IParameter* param, T defaultValue) : TypeDefinition<type>(param), m_defaultValue(defaultValue){}


protected:
    virtual bool handleOption(uint8_t optionId, std::istream& is) override
    {
        if (optionId == defaultprefix)
        {
            m_defaultValue = parseValue(is);
            return true;
        }

        return false;
    }


protected:
   Option<T> m_defaultValue{};
};

}


#endif // TYPEDEFINITIONDEFAULT_H
