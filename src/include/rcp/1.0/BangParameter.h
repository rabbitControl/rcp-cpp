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

#ifndef RCP_V10_BANGPARAMETER_H
#define RCP_V10_BANGPARAMETER_H

#include "TypeDefinitions.h"
#include "Parameter.h"

namespace rcp {
namespace v10 {


class BangParameter;
using BangParameterPtr = std::shared_ptr<BangParameter>;



class BangParameter
    : public Parameter<TypeDefinitionBang>
{
public:
    static BangParameterPtr create(uint32_t id)
    {
        class make_shared_enabler : public BangParameter
        {
            public: make_shared_enabler(uint32_t id)
                    : BangParameter(id)
                    , rcp::Parameter<TypeDefinitionBang>(id)
                    , rcp::IParameter(id)
                {}
        };
        return std::make_shared<make_shared_enabler>(id);
    }

public:
    void setBangCb(const std::function<void()>& fn)
    {
        m_cb = fn;
    }

    virtual void update(rcp::IParameterPtr other) override
    {
        if (other.get() != this &&
            this->m_id == other->getId() &&
            this->getDatatype() == other->getDatatype() &&
            !other->anyOptionChanged())
        {
            if (m_cb)
            {
                m_cb();
            }
            return;
        }

        Parameter<TypeDefinitionBang>::update(other);
    }

    void bang()
    {
        setDirty();
    }

private:
    BangParameter(uint32_t id)
        : Parameter<TypeDefinitionBang>(id)
        , rcp::Parameter<TypeDefinitionBang>(id)
        , rcp::IParameter(id)
    {
        if (id > RcpInt::MAX_VALUE)
        {
            throw std::runtime_error("invalid parameter id");
        }
    }

private:
    std::function<void()> m_cb;
};


}
}

#endif // BANGPARAMETER_H
