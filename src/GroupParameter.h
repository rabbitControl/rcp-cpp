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

#ifndef RCP_GROUPPARAMETER_H
#define RCP_GROUPPARAMETER_H


#include <map>

#include "IGroupParameter.h"

namespace rcp
{


class GroupParameter
    : public IGroupParameter
{

public:
    // IGroupParameter
    void addChild(IParameterPtr param) override
    {
        m_children[param->getId()] = param;
        param->setParent(shared());
    }

    void addChildInternal(IParameterPtr param) override
    {
        m_children[param->getId()] = param;
        param->setParentInternal(shared());
    }

    void removeChild(IParameterPtr param) override
    {
        auto it = m_children.find(param->getId());

        if (it != m_children.end())
        {
            m_children.erase(it);
            param->clearParentInternal();
        }
        else
        {
            // std::cout << this->label << ": child not found!" << std::endl;
        }
    }

    void clear() override
    {
        for (auto& child : m_children)
        {
            child.second->clearParentInternal();
        }

        m_children.clear();
    }

    virtual std::map<IdType, IParameterPtr> children() const override
    {
        return m_children;
    }



    // child as ...
    template<typename Target>
    std::shared_ptr<Target> childAs(const IdType id) const
    {
        auto it = m_children.find(id);
        if (it != m_children.end())
        {
            auto r = std::dynamic_pointer_cast<Target>(it->second);
            if (r)
            {
                return r;
            }
        }

        return nullptr;
    }


protected:
    std::map<IdType, IParameterPtr> m_children;

protected:
    virtual inline IGroupParameterPtr shared() = 0;
};

}


#endif // GROUPPARAMETER_H
