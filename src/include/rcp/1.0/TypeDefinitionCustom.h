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

#ifndef RCP_V10_TYPEDEFINITIONCUSTOM_H
#define RCP_V10_TYPEDEFINITIONCUSTOM_H

#include "../TypeDefinitionDefault.h"

#include "RcpInt.h"
#include "types.h"

namespace rcp {
namespace v10 {


class TypeDefinitionCustom
    : public rcp::TypeDefinitionDefault<std::vector<char>, DATATYPE_CUSTOMTYPE, OptionsDefault>
{
public:
    TypeDefinitionCustom(rcp::IParameter* param)
        : rcp::TypeDefinitionDefault<std::vector<char>, DATATYPE_CUSTOMTYPE, OptionsDefault>(param)
    {}

    uint32_t getSize() const
    {
        return size;
    }


public:
    virtual std::vector<char> parseValue(std::istream& is) override
    {
        std::vector<char> data;
        data.resize(size);
        is.read(data.data(), size);

        return data;
    }

    virtual void writeValue(Writer& out, const std::vector<char>& value) override
    {
        out.write(value.data(), value.size());
    }

    virtual void write(Writer& out, bool all) override
    {
        auto write_default = (all && this->m_defaultValue.hasValue()) || this->m_defaultValue.changed();
        auto write_typeId = (all && this->typeId.hasValue()) || this->typeId.changed();
        auto write_config = (all && this->hasConfig()) || this->configChanged;

        out.write(static_cast<char>(datatype() | (write_default || write_typeId || write_config ? 0x00 : Terminator)));

        RcpInt(size).write(out, true);


        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT | (write_typeId || write_config ? 0x00 : Terminator)));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT | (write_typeId || write_config ? 0x00 : Terminator)));
            out.write(static_cast<char>(Terminator)); // RcpInt(0)
            m_defaultValue.setUnchanged();
        }


        // type id
        if (typeId.hasValue())
        {
            if (all || typeId.changed())
            {
                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_TYPEID | (write_config ? 0x00 : Terminator)));

                RcpInt(typeId.value()).write(out, true);

                if (!all)
                {
                    typeId.setUnchanged();
                }
            }

        }
        else if (typeId.changed())
        {
            out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_TYPEID | (write_config ? 0x00 : Terminator)));
            out.write(static_cast<char>(Terminator)); // RcpInt(0)
            typeId.setUnchanged();
        }


        // config
        if (config.size() > 0)
        {
            if (all || configChanged)
            {
                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_CONFIG | Terminator));

                RcpInt(config.size()).write(out, true);

                // write
                out.write((char*)config.data(), config.size());

                if (!all)
                {
                    configChanged = false;
                }
            }

        }
        else if (configChanged)
        {
            out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_CONFIG | Terminator));
            out.write(static_cast<char>(Terminator)); // RcpInt(0)
            configChanged = false;
        }
    }

    virtual bool handleOption(uint8_t optionId, std::istream& is) override
    {
        switch (optionId)
        {
        case CUSTOMTYPE_OPTIONS_TYPEID:
        {
            typeId = RcpInt::parse(is).value();
            return true;
        }
        case CUSTOMTYPE_OPTIONS_CONFIG:
        {
            uint32_t data_length = RcpInt::parse(is).value();
            CHECK_STREAM;

            std::vector<char> data;
            data.resize(data_length);
            is.read(data.data(), data_length);

            CHECK_STREAM;

            config = data;

            return true;
        }
        }

        return TypeDefinitionDefault::handleOption(optionId, is);
    }



    //------------------------------------
    //

    // uuid
    bool hasTypeId() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return typeId.hasValue();
    }

    void setTypeId(uint32_t type)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        updateOption(typeId, type);
    }
    const uint32_t getTypeId() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return typeId.valueOr(0);
    }
    void clearTypeId()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        clearOption(typeId);
    }


    // config
    bool hasConfig() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return config.size() > 0;
    }
    const std::vector<char>& getConfig() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return config;
    }
    void setConfig(const std::vector<char>& config)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        if (this->config != config)
        {
            this->config = config;
            configChanged = true;
            setDirty();
        }
    }
    void clearConfig()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        if (!config.empty())
        {
            config.clear();
            configChanged = true;
            setDirty();
        }
    }


    virtual void setAllUnchanged() override
    {
        TypeDefinitionDefault::setAllUnchanged();
        typeId.setUnchanged();
        configChanged = false;
    }


private:
    friend class CustomParameter;
    void setSize(uint32_t s)
    {
        size = s;
    }

private:
    uint32_t size{0};

    Option<uint32_t> typeId;

    std::vector<char> config;
    bool configChanged{false};

};


}
}


#endif // TYPEDEFINITIONCUSTOM_H
