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

#ifndef RCP_V010_BANGPARAMETER_H
#define RCP_V010_BANGPARAMETER_H

#include "../Parameter.h"

#include "TypeDefinitionBang.h"

namespace rcp {
namespace v010 {


class BangParameter;
using BangParameterPtr = std::shared_ptr<BangParameter>;



class BangParameter
    : public Parameter<TypeDefinitionBang>
{
public:
    static BangParameterPtr create(int16_t id)
    {
        class make_shared_enabler
            : public BangParameter
        {
            public: make_shared_enabler(int16_t id)
                    : BangParameter(id)
                    , IParameter(id)
                {}
        };
        return std::make_shared<make_shared_enabler>(id);
    }

public:
    void setBangCb(const std::function<void()>& fn)
    {
        m_cb = fn;
    }

    virtual void update(IParameterPtr other) override
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
    BangParameter(int16_t id)
        : Parameter<TypeDefinitionBang>(id)
        , IParameter(id)
    {}

private:
    std::function<void()> m_cb;
};


}
}

#endif // BANGPARAMETER_H
