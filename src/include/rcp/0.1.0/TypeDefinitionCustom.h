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

#ifndef RCP_V010_TYPEDEFINITIONCUSTOM_H
#define RCP_V010_TYPEDEFINITIONCUSTOM_H

#include "../TypeDefinitionDefault.h"

#include "types.h"

namespace rcp {
namespace v010 {


class TypeDefinitionCustom
    : public rcp::TypeDefinitionDefault<std::vector<char>, v010::DATATYPE_CUSTOMTYPE, v010::OptionsDefault>
{
public:
    TypeDefinitionCustom(IParameter* param)
        : rcp::TypeDefinitionDefault<std::vector<char>, v010::DATATYPE_CUSTOMTYPE, v010::OptionsDefault>(param)
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
        out.write(datatype());

        out.write(size);


        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT));
            out.write(static_cast<int32_t>(0));
            m_defaultValue.setUnchanged();
        }


        // uuid
        if (m_hasUuid)
        {
            if (all || uuidChanged)
            {
                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_UUID));

                out.write(uuid, 16);

                if (!all)
                {
                    uuidChanged = false;
                }
            }

        }
        else if (uuidChanged)
        {
            out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_UUID));

            // if uuid was cleared we write 0-bytes
            char z_uuid[16];
            memset(z_uuid, 0, 16);
            out.write(z_uuid, 16);
            uuidChanged = false;
        }


        // config
        if (config.size() > 0)
        {
            if (all || configChanged)
            {
                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_CONFIG));

                out.write(static_cast<uint32_t>(config.size()));

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
            out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_CONFIG));
            out.write(static_cast<uint32_t>(0));
            configChanged = false;
        }


        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }

    virtual bool handleOption(uint8_t optionId, std::istream& is) override
    {
        switch (optionId)
        {
        case CUSTOMTYPE_OPTIONS_UUID:
        {
            char uuid[16];
            is.read(uuid, 16);
            CHECK_STREAM;

            m_hasUuid = true;
            memcpy(uuid, uuid, 16);

            return true;
        }
        case CUSTOMTYPE_OPTIONS_CONFIG:
        {
            uint32_t data_length = readFromStream(is, data_length);
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
    bool hasUuid() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return m_hasUuid;
    }

    void setUuid(const char* uuid, uint8_t length)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        if (length != 16) {
            return;
        }

        m_hasUuid = true;
        if (memcmp(uuid, uuid, 16) == 0)
        {
            return;
        }

        memcpy(this->uuid, uuid, 16);
        uuidChanged = true;
        setDirty();
    }
    const char* getUuid() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif
        return uuid;
    }
    void clearUuid()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(m_parameter->mutex());
#endif

        memset(uuid, 0, 16);
        m_hasUuid = false;
        uuidChanged = true;
        setDirty();
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
        uuidChanged = false;
        configChanged = false;
    }


private:
    friend class CustomParameter;
    void setSize(uint32_t s)
    {
        size = s;
    }

private:
    uint32_t size;

    char uuid[16];
    bool m_hasUuid;
    bool uuidChanged;

    std::vector<char> config;
    bool configChanged;

};


}
}


#endif // TYPEDEFINITIONCUSTOM_H
