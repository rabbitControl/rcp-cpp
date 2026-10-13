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

#ifndef RCP_V10_ENUMTYPEDEFINITION_H
#define RCP_V10_ENUMTYPEDEFINITION_H

#include "../TypeDefinitionDefault.h"

#include "RcpLanguageString.h"
#include "IParameter.h"
#include "ParameterParser.h"
#include "ParameterWriter.h"
#include "RcpInt.h"
#include "types.h"

namespace rcp {
namespace v10 {


class TypeDefinitionEnum
    : public rcp::TypeDefinitionDefault<std::vector<uint32_t>, DATATYPE_ENUM, OptionsDefault>
{
public:
    TypeDefinitionEnum(rcp::IParameter* param)
        : rcp::TypeDefinitionDefault<std::vector<uint32_t>, DATATYPE_ENUM, OptionsDefault>(param)
    {}

    virtual std::vector<uint32_t> parseValue(std::istream& is) override
    {
        std::vector<uint32_t> indices;

        auto count = RcpInt::parse(is).value();

        for (int var = 0; var < count; ++var)
        {
            indices.push_back(RcpInt::parse(is).value());
        }

        return indices;
    }

    virtual void writeValue(Writer& out, const std::vector<uint32_t>& value) override
    {
        RcpInt(value.size()).write(out, true);

        for (auto& v : value)
        {
            RcpInt(v).write(out, true);
        }
    }

    virtual void write(Writer& out, bool all) override
    {
        auto write_default = (all && this->m_defaultValue.hasValue()) || this->m_defaultValue.changed();
        auto write_entries = (all && this->m_entries.size() > 0) || this->m_entriesChanged;
        auto write_min = (all && this->m_minimumSelectionCount.hasValue()) || this->m_minimumSelectionCount.changed();
        auto write_max = (all && this->m_maximumSelectionCount.hasValue()) || this->m_maximumSelectionCount.changed();

        auto all_or = write_default || write_min || write_max || write_entries;

        out.write(static_cast<char>(this->datatype() | (all_or ? 0x00 : Terminator)));


        all_or = write_min || write_max || write_entries;

        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(ENUM_OPTIONS_DEFAULT | (all_or ? 0x00 : Terminator)));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(ENUM_OPTIONS_DEFAULT | (all_or ? 0x00 : Terminator)));
            writeValue(out, {});
            m_defaultValue.setUnchanged();
        }


        all_or = write_min || write_max;

        // entries
        if (m_entries.size() > 0)
        {
            if (all || m_entriesChanged)
            {
                out.write(static_cast<char>(ENUM_OPTIONS_ENTRIES | (all_or ? 0x00 : Terminator)));

                RcpInt(m_entries.size()).write(out, true);

                for (auto& entry : m_entries)
                {
                    entry.write(out, true);
                }

                if (!all)
                {
                    m_entriesChanged = false;
                }
            }

        }
        else if (m_entriesChanged)
        {
            out.write(static_cast<char>(ENUM_OPTIONS_ENTRIES | (all_or ? 0x00 : Terminator)));
            out.write(static_cast<char>(Terminator)); // RcpInt(0)
            m_entriesChanged = false;
        }


        // minimumSelectionCount

        all_or = write_max;

        if (m_minimumSelectionCount.hasValue())
        {
            if (all || m_minimumSelectionCount.changed())
            {
                out.write(static_cast<char>(ENUM_OPTIONS_MINIMUM_SELECTION_COUNT | (all_or ? 0x00 : Terminator)));

                RcpInt(m_minimumSelectionCount.value()).write(out, true);

                if (!all)
                {
                    m_minimumSelectionCount.setUnchanged();
                }
            }

        }
        else if (m_minimumSelectionCount.changed())
        {
            out.write(static_cast<char>(ENUM_OPTIONS_MINIMUM_SELECTION_COUNT | (all_or ? 0x00 : Terminator)));
            out.write(static_cast<char>(Terminator)); // RcpInt(0)
            m_minimumSelectionCount.setUnchanged();
        }


        // maximumSelectionCount

        if (m_maximumSelectionCount.hasValue())
        {
            if (all || m_maximumSelectionCount.changed())
            {
                out.write(static_cast<char>(ENUM_OPTIONS_MAXIMUM_SELECTION_COUNT | Terminator));

                RcpInt(m_maximumSelectionCount.value()).write(out, true);

                if (!all)
                {
                    m_maximumSelectionCount.setUnchanged();
                }
            }

        }
        else if (m_maximumSelectionCount.changed())
        {
            out.write(static_cast<char>(ENUM_OPTIONS_MAXIMUM_SELECTION_COUNT | Terminator));
            RcpInt(1).write(out, true);
            m_maximumSelectionCount.setUnchanged();
        }


    }



    bool handleOption(uint8_t optionId, std::istream &is) override
    {
        switch (optionId)
        {
        case ENUM_OPTIONS_DEFAULT:
        {
            auto items = parseValue(is);
            CHECK_STREAM;

            m_defaultValue = items;

            return true;
        }
        case ENUM_OPTIONS_ENTRIES:
        {
            m_entries.clear();

            auto entry_count = RcpInt::parse(is).value();

            CHECK_STREAM;

            for (int i = 0; i < entry_count; ++i)
            {
                auto lng = RcpLanguageString::parse(is);
                m_entries.push_back(lng);
            }

            return true;
        }

        case ENUM_OPTIONS_MINIMUM_SELECTION_COUNT:
        {
            uint32_t v = RcpInt::parse(is).value();
            CHECK_STREAM;

            m_minimumSelectionCount = v;

            return true;
        }

        case ENUM_OPTIONS_MAXIMUM_SELECTION_COUNT:
        {
            uint32_t v = RcpInt::parse(is).value();
            CHECK_STREAM;

            m_maximumSelectionCount = v;

            return true;
        }
        }

        return false;
    }



    //--------
    // m_entries
    std::vector<std::string> getLanguageOptions(const std::string& code) const
    {
        if (code.size() != 3)
        {
            return {};
        }

#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        std::vector<std::string> result;

        for (const auto& lng : m_entries)
        {
            auto it = lng.languageMap().find(code);
            if (it != lng.languageMap().end())
            {
                result.push_back(it->second);
            }
            else
            {
                result.push_back("");
            }
        }

        return result;
    }

    std::vector<std::string> getAnyOptions() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        std::vector<std::string> result;

        for (auto& lng : m_entries)
        {
            if (lng.hasAnyString())
            {
                result.push_back(lng.anyString());
            }
            else
            {
                result.push_back("");
            }
        }

        return result;
    }

    RcpLanguageString getOption(int index) const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        if (index < m_entries.size())
        {
            return m_entries[index];
        }

        return RcpLanguageString{};
    }
    void addOption(const RcpLanguageString& option)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        m_entries.push_back(option);
        m_entriesChanged = true;
        setDirty();
    }
    void setOptions(const std::vector<RcpLanguageString>& options)
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
    // m_minimumSelectionCount
    uint32_t getMinimumSelectionCount() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        return m_minimumSelectionCount.valueOr(0);
    }
    bool hasMinimumSelectionCount() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return m_minimumSelectionCount.hasValue();
    }
    void setMinimumSelectionCount(uint32_t value)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        updateOption(m_minimumSelectionCount, value);
    }
    void clearMinimumSelectionCount()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        clearOption(m_minimumSelectionCount);
    }


    //------------
    // m_maximumSelectionCount
    uint32_t getMaximumSelectionCount() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        return m_maximumSelectionCount.valueOr(1);
    }
    bool hasMaximumSelectionCount() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return m_maximumSelectionCount.hasValue();
    }
    void setMaximumSelectionCount(uint32_t value)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        updateOption(m_maximumSelectionCount, value);
    }
    void clearMaximumSelectionCount()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        clearOption(m_maximumSelectionCount);
    }

    virtual void setAllUnchanged() override
    {
        TypeDefinitionDefault::setAllUnchanged();
        m_entriesChanged = false;
        m_minimumSelectionCount.setUnchanged();
        m_maximumSelectionCount.setUnchanged();
    }


private:

    // m_entries - enum
    std::vector<RcpLanguageString> m_entries;
    bool m_entriesChanged{false};

    Option<uint32_t> m_minimumSelectionCount;
    Option<uint32_t> m_maximumSelectionCount;
};


}
}

#endif // ENUMTYPEDEFINITION_H
