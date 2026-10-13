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

#ifndef RCP_V010_ENUMTYPEDEFINITION_H
#define RCP_V010_ENUMTYPEDEFINITION_H


#include "../TypeDefinitionDefault.h"
#include "../IParameter.h"

#include "types.h"
#include "v010StreamTools.h"

namespace rcp {
namespace v010 {


class TypeDefinitionEnum
    : public TypeDefinitionDefault<std::string, v010::DATATYPE_ENUM, v010::OptionsDefault>
{
public:
    TypeDefinitionEnum(IParameter* param) : TypeDefinitionDefault<std::string, v010::DATATYPE_ENUM, v010::OptionsDefault>(param) {}

    virtual std::string parseValue(std::istream& is) override
    {
        return readStringFromStream(is, uint8_t{});
    }

    virtual void writeValue(Writer& out, const std::string& value) override
    {
        ParameterWriter::writeTinyString(out, value);
    }

    virtual void write(Writer& out, bool all) override
    {
        out.write(this->datatype());


        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(ENUM_OPTIONS_DEFAULT));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(ENUM_OPTIONS_DEFAULT));
            writeValue(out, empty_string);
            m_defaultValue.setUnchanged();
        }


        // options
        if (m_entries.size() > 0)
        {
            if (all || m_entriesChanged)
            {
                out.write(static_cast<char>(ENUM_OPTIONS_ENTRIES));

                for (const std::string& entry : m_entries)
                {
                    ParameterWriter::writeTinyString(out, entry);
                }

                out.write(static_cast<uint8_t>(TERMINATOR));

                if (!all)
                {
                    m_entriesChanged = false;
                }
            }

        }
        else if (m_entriesChanged)
        {
            out.write(static_cast<char>(ENUM_OPTIONS_ENTRIES));
            out.write(static_cast<uint8_t>(TERMINATOR));
            m_entriesChanged = false;
        }


        // multiselect
        if (m_multiselect.hasValue())
        {
            if (all || m_multiselect.changed())
            {
                out.write(static_cast<char>(ENUM_OPTIONS_MULTISELECT));
                out.write(m_multiselect.value());

                if (!all)
                {
                    m_multiselect.setUnchanged();
                }
            }

        }
        else if (m_multiselect.changed())
        {

            out.write(static_cast<char>(ENUM_OPTIONS_MULTISELECT));
            out.write(false);
            m_multiselect.setUnchanged();
        }


        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }



    bool handleOption(uint8_t optionId, std::istream &is) override
    {
        switch (optionId)
        {
        case ENUM_OPTIONS_DEFAULT:
        {
            std::string d = ParameterParser::readTinyString(is);
            CHECK_STREAM;

            m_defaultValue = d;

            return true;
        }
        case ENUM_OPTIONS_ENTRIES:
        {
            m_entries.clear();

            std::string option = ParameterParser::readTinyString(is);

            CHECK_STREAM;

            while (option != "")
            {
                m_entries.push_back(option);

                option = ParameterParser::readTinyString(is);

                CHECK_STREAM;
            }

            return true;
        }
        case ENUM_OPTIONS_MULTISELECT:
        {
            bool multi = readFromStream(is, multi);
            CHECK_STREAM;

            m_multiselect = multi;

            return true;
        }
        }

        return false;
    }



    //--------
    // m_entries
    const std::vector<std::string>& getOptions() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return m_entries;
    }


    // ??
    // deprecated
    const std::string& getOption(const std::string& selection) const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        auto it = std::find(m_entries.begin(), m_entries.end(), selection);
        if (it != m_entries.end()) {
            return *it;
        }

        return empty_string;
    }
    void addOption(const std::string& option)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        m_entries.push_back(option);
        m_entriesChanged = true;
        setDirty();
    }
    void setOptions(const std::vector<std::string>& options)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        this->m_entries = options;
        m_entriesChanged = true;
        setDirty();
    }
    bool hasOptions() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return m_entries.size() > 0;
    }
    void clearOptions()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        if (!m_entries.empty())
        {
            m_entries.clear();
            m_entriesChanged = true;
            setDirty();
        }
    }

    //------------
    // m_multiselect
    bool getMultiselect() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        if (m_multiselect.hasValue())
        {
            return m_multiselect.value();
        }
        return false;
    }
    bool hasMultiselect() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return m_multiselect.hasValue();
    }
    void setMultiselect(bool value)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        updateOption(m_multiselect, value);
    }
    void clearMultiselect()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        clearOption(m_multiselect);
    }

    virtual void setAllUnchanged() override
    {
        TypeDefinitionDefault::setAllUnchanged();
        m_entriesChanged = false;
        m_multiselect.setUnchanged();
    }


private:

    // m_entries - enum
    std::vector<std::string> m_entries;
    bool m_entriesChanged;

    Option<bool> m_multiselect;

};


}
}

#endif // ENUMTYPEDEFINITION_H
