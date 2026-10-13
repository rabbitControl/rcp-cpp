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

#ifndef RCP_URITYPEDEFINITION_H
#define RCP_URITYPEDEFINITION_H

#include "TypeDefinitionDefault.h"

namespace rcp {


template<uint8_t type,
         uint8_t defaultprefix>
class TypeDefinitionURI
    : public TypeDefinitionDefault<std::string, type, defaultprefix>
{
public:
    TypeDefinitionURI(IParameter* param)
        : TypeDefinitionDefault<std::string, type, defaultprefix>(param)
    {}


    virtual void setAllUnchanged() override
    {
        rcp::TypeDefinitionDefault<std::string, type, defaultprefix>::setAllUnchanged();
        m_filter.setUnchanged();
        m_schemaChanged = false;
    }



public:
    //----------------------
    // options - filter
    std::string getFilter() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return m_filter.value();
    }
    bool hasFilter() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return m_filter.hasValue();
    }
    void setFilter(const std::string& filter)
    {
        this->updateOption(m_filter, filter);
    }
    void clearFilter()
    {
        this->clearOption(m_filter);
    }

    //----------------------
    // options - schemas
    std::vector<std::string> getSchemas() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif
        return m_schemas;
    }
    void addSchema(const std::string& schema)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (std::find(m_schemas.begin(), m_schemas.end(), schema) != m_schemas.end())
        {
            return;
        }

        m_schemas.push_back(schema);
        m_schemaChanged = true;
        this->setDirty();
    }
    void setSchemas(const std::vector<std::string>& schemas)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (m_schemas != schemas)
        {
            m_schemas = schemas;
            m_schemaChanged = true;
            this->setDirty();
        }
    }
    void clearSchemas()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(this->m_parameter->mutex());
#endif

        if (!m_schemas.empty())
        {
            m_schemas.clear();
            m_schemaChanged = true;
            this->setDirty();
        }
    }


protected:
    Option<std::string> m_filter;

    std::vector<std::string> m_schemas;
    bool m_schemaChanged{false};
};


}

#endif // URITYPEDEFINITION_H
