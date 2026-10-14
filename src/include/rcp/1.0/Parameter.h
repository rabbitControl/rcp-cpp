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

#ifndef RCP_V10_PARAMETER_H
#define RCP_V10_PARAMETER_H

#include "../Parameter.h"

#include "types.h"
#include "IParameter.h"

namespace rcp {
namespace v10 {

template<typename TD>
class Parameter
    : public IParameter
    , public virtual rcp::Parameter<TD>
{

public:
    // IParameter

    // readonly
    virtual bool getEnabled() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_mutex);
#endif

        return enabled.valueOr(true);
    }

    virtual void setEnabled(const bool enabled) override
    {
        if (this->updateOption(this->enabled, enabled, v10::PARAMETER_OPTIONS_ENABLED))
        {
            // call listener
            for (auto listener : this->m_parameterListener)
            {
                listener->onEnabledChanged(*this);
            }
        }
    }

    virtual bool hasEnabled() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_mutex);
#endif
        return enabled.hasValue();
    }

    virtual void clearEnabled() override
    {
        if (this->clearOption(enabled, v10::PARAMETER_OPTIONS_ENABLED))
        {
            // call listener
            for (auto listener : this->m_parameterListener)
            {
                listener->onEnabledChanged(*this);
            }
        }
    }



    virtual void update(rcp::IParameterPtr other) override
    {
        if (other.get() == this)
        {
            return;
        }

        if (m_id != other->getId())
        {
            std::cerr << "update from wrong id" << std::endl;
            return;
        }

        auto typed_param = std::dynamic_pointer_cast<Parameter<TD>>(other);
        if (typed_param)
        {
            // update parameter
            bool updated = false;


            if (typed_param->hasEnabled())
            {
                auto old_value = getEnabled();

                if (old_value != typed_param->getEnabled())
                {
                    enabled.setValue(typed_param->getEnabled());

                    updated = true;

                    // call listener
                    for (auto listener : this->m_parameterListener)
                    {
                        listener->onEnabledChanged(*this);
                    }
                }

                enabled.setUnchanged();

                // NOTE: parameter-option ids stay the same between v010 and v10
                this->m_changed.erase(v10::PARAMETER_OPTIONS_ENABLED);
            }


            // check other options
            updated |= rcp::Parameter<TD>::updateInternal(typed_param);

            if (updated)
            {
                for (auto& fn : this->m_updatedCallbacks)
                {
                    fn.second();
                }
            }
        }
    }



public:
    Parameter(uint32_t id, const std::string& label = "")
        : rcp::Parameter<TD>(id, label)
        , rcp::IParameter(id, label)
    {}


};

}
}

#endif // RCP_V10_PARAMETER_H
