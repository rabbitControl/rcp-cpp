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

#ifndef RCP_VALUEPARAMETER_H
#define RCP_VALUEPARAMETER_H

#include <functional>
#include <unordered_map>

#include "Option.h"


#define VALUEPARAMDEF(ARG, T) \
using ARG##Parameter = ValueParameter<T, TypeDefinition##ARG>;\
using ARG##ParameterPtr = std::shared_ptr<ARG##Parameter>;


namespace rcp {


template<typename T>
class ValueParameter
{

public:
    T getValue() const
    {
        if (m_value.hasValue())
        {
            return m_value.value();
        }

        return {};
    }

    virtual void setValue(T value) = 0;

    bool hasValue() const
    {
        return m_value.hasValue();
    }


    void addValueUpdatedCb(void* obj, const std::function<void(const T&)>& fn)
    {
        m_valueUpdatedCallbacks[obj] = fn;
    }

    void removeValueUpdatedCb(void* obj)
    {
        if (m_valueUpdatedCallbacks.find(obj) != m_valueUpdatedCallbacks.end())
        {
            m_valueUpdatedCallbacks.erase(obj);
        }
    }

    void clearValueUpdatedCb()
    {
        m_valueUpdatedCallbacks.clear();
    }

protected:
    Option<T> m_value;

    std::unordered_map<void*, std::function<void(const T&)>> m_valueUpdatedCallbacks;
};

}


#endif // VALUEPARAMETER_H
