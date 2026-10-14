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

#ifndef RCP_V10_VALUEPARAMETER_H
#define RCP_V10_VALUEPARAMETER_H

#include "../ValueParameter.h"

#include "Parameter.h"
#include "RcpInt.h"
#include "types.h"

namespace rcp {
namespace v10 {


template<typename T,
         typename TD> class ValueParameter;

template<typename T,
         typename TD>
using ValueParameterPtr = std::shared_ptr<ValueParameter<T, TD>>;



template<typename T,
         typename TD>
class ValueParameter
    : public Parameter<TD>
    , public rcp::ValueParameter<T>
{
public:
    static v10::ValueParameterPtr<T, TD> create(uint32_t id)
    {
        class make_shared_enabler : public ValueParameter<T, TD>
        {
            public: make_shared_enabler(int16_t id)
                    : ValueParameter<T, TD>(id)
                    , rcp::Parameter<TD>(id)
                    , rcp::IParameter(id)
                {}
        };

        return std::make_shared<make_shared_enabler>(id);
    }

public:
    // ValueParameter
    void setValue(T value) override
    {
        this->m_value = value;

        if (this->m_value.changed())
        {
            // NOTE: parameter-option ids stay the same between v010 and v10
            this->m_changed.insert(v010::PARAMETER_OPTIONS_VALUE);

            this->setDirty();
        }
    }


public:
    // Parameter
    virtual void update(rcp::IParameterPtr other) override
    {
        if (other.get() == this)
        {
            return;
        }

        if (Parameter<TD>::getId() != other->getId())
        {
            std::cerr << "update from wrong id" << std::endl;
            return;
        }

        auto v_other = std::dynamic_pointer_cast<ValueParameter<T, TD>>(other);
        if (v_other)
        {
            // type match

            if (v_other->hasValue())
            {
                this->m_value = v_other->getValue();
                if (this->m_value.changed())
                {
                    auto v = this->m_value.value();

                    for (auto& f : this->m_valueUpdatedCallbacks)
                    {
                        f.second(v);
                    }
                }

                // NOTE: parameter-option ids stay the same between v010 and v10
                this->m_changed.erase(v010::PARAMETER_OPTIONS_VALUE);
            }

            Parameter<TD>::update(other);
        }
        else
        {
            // wrong type?
        }
    }

    virtual bool isValueParameter() const override
    {
        return true;
    }

    virtual void writeValue(Writer &out) override
    {
        this->m_typedefinition->writeValue(out, this->m_value.value());
    }

    virtual void writeValueOption(Writer &out, bool all, uint8_t id) override
    {
        // value
        if (this->m_value.hasValue())
        {
            if (all || this->m_value.changed())
            {
                out.write(id);

                this->m_typedefinition->writeValue(out, this->m_value.value());

                if (!all)
                {
                    this->m_value.setUnchanged();
                }
            }
        }
        else if (this->m_value.changed())
        {
            out.write(id);

            this->m_typedefinition->writeValue(out, type_zero(T{}));

            this->m_value.setUnchanged();
        }
    }

    virtual bool hasValueOption() override
    {
        return this->m_value.hasValue();
    }

    virtual bool handleOption(uint8_t option, std::istream& is) override
    {
        if (option == PARAMETER_OPTIONS_VALUE)
        {
            this->m_value = this->m_typedefinition->parseValue(is);

            return true;
        }

        return false;
    }


protected:
    ValueParameter(uint32_t id)
        : Parameter<TD>(id)
        , rcp::Parameter<TD>(id)
        , rcp::IParameter(id)
    {
        if (id > RcpInt::MAX_VALUE)
        {
            throw std::runtime_error("invalid parameter id");
        }
    }
};


}
}

#endif // VALUEPARAMETER_H
