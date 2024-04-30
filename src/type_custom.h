/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef TYPE_CUSTOM_H
#define TYPE_CUSTOM_H

#include <iostream>
#include <vector>
#include <string.h>

#include "typedefinition.h"
#include "iparameter.h"
#include "stream_tools.h"

namespace rcp {

typedef TypeDefinition<std::vector<char>, DATATYPE_CUSTOMTYPE, td_custom > CustomTypeDefinition;

template<>
class TypeDefinition<std::vector<char>, DATATYPE_CUSTOMTYPE, td_custom>
    : public IDefaultDefinition<std::vector<char>>
{
public:
    TypeDefinition(TypeDefinition<std::vector<char>, DATATYPE_CUSTOMTYPE, td_custom>& v) :
        obj(v.obj)
    {}

    TypeDefinition(const TypeDefinition<std::vector<char>, DATATYPE_CUSTOMTYPE, td_custom>& v) :
        obj(v.obj)
    {}

    TypeDefinition(IParameter& param) :
        obj(std::make_shared<Value>(param))
    {}

    TypeDefinition(const std::vector<char>& d, IParameter& param) :
        obj(std::make_shared<Value>(d, param))
    {}


    //------------------------------------
    // Writeable
    void write(Writer& out, bool all) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->write(out, all);

        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }


    //------------------------------------
    // IOptionparser
    void parseOptions(std::istream& is) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        while (!is.eof())
        {
            // read option prefix
            customtype_options_t opt = static_cast<customtype_options_t>(is.get());

            if (is.eof()) {
                break;
            }

            if (opt == TERMINATOR) {
                break;
            }

            switch (opt) {
            case CUSTOMTYPE_OPTIONS_DEFAULT:
            {
                std::vector<char> data;
                data.resize(obj->size);
                is.read(data.data(), obj->size);

                CHECK_STREAM;

                obj->defaultValue = data;

                break;
            }
            case CUSTOMTYPE_OPTIONS_UUID:
            {
                char uuid[16];
                is.read(uuid, 16);
                CHECK_STREAM;

                obj->hasUuid = true;
                memcpy(obj->uuid, uuid, 16);
                break;
            }
            case CUSTOMTYPE_OPTIONS_CONFIG:
            {
                uint32_t data_length = readFromStream(is, data_length);
                CHECK_STREAM;

                std::vector<char> data;
                data.resize(data_length);
                is.read(data.data(), data_length);

                CHECK_STREAM;

                obj->config = data;

                break;
            }
            }
        }
    }


    //------------------------------------
    // ITypeDefinition

    datatype_t getDatatype() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->datatype;
    }

    bool anyOptionChanged() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        return obj->defaultValueChanged
               || obj->uuidChanged
               || obj->configChanged;
    }

    void writeMandatory(Writer& out) const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        obj->writeMandatory(out);
    }

    void dump() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        std::cout << "--- type custom ---\n";

        if (hasDefault())
        {
            std::cout << "\tdefault: " << value_to_string(getDefault()) << "\n";
        }

        if (hasUuid())
        {
            std::cout << "\tuuid: " << getUuid() << "\n";
        }

        if (hasConfig())
        {
            std::cout << "\tconfig: " << value_to_string(getConfig()) << "\n";
        }
    }


    //------------------------------------
    // IDefaultDefinition<T>

    std::vector<char> readValue(std::istream& is) override
    {
        std::vector<char> data(obj->size);
        is.read(data.data(), obj->size);
        return data;
    }

    //--------
    // default
    const std::vector<char> getDefault() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->defaultValue;
    }
    void setDefault(const std::vector<char>& defaultValue) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue = defaultValue;
        obj->defaultValueChanged = true;
        setDirty();
    }
    bool hasDefault() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return !obj->defaultValue.empty();
    }
    void clearDefault() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (!obj->defaultValue.empty())
        {
            obj->defaultValue.clear();
            obj->defaultValueChanged = true;
            setDirty();
        }
    }

    //------------------------------------
    //

    // uuid
    bool hasUuid() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->hasUuid;
    }
    void setUuid(const char* uuid, uint8_t length)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (length != 16) {
            return;
        }

        obj->hasUuid = true;
        if (memcmp(obj->uuid, uuid, 16) == 0)
        {
            return;
        }

        memcpy(obj->uuid, uuid, 16);
        obj->uuidChanged = true;
        setDirty();
    }
    const char* getUuid() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->uuid;
    }
    void clearUuid()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        memset(obj->uuid, 0, 16);
        obj->hasUuid = false;
        obj->uuidChanged = true;
        setDirty();
    }


    // config
    bool hasConfig() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->config.size() > 0;
    }
    const std::vector<char>& getConfig() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->config;
    }
    void setConfig(const std::vector<char>& config)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->config = config;
        obj->configChanged = true;
        setDirty();
    }
    void clearConfig()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->config.clear();
        obj->configChanged = true;
        setDirty();
    }

    void setAllUnchanged() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValueChanged = false;
        obj->uuidChanged = false;
        obj->configChanged = false;
    }

    uint32_t getSize() const
    {
        return obj->size;
    }

private:
    void setDirty() {
        obj->parameter.setDirty();
    }

    friend class CustomParameter;
    void setSize(uint32_t size)
    {
        obj->size = size;
    }

    class Value {
    public:
        Value(IParameter& param)
            : datatype(DATATYPE_CUSTOMTYPE)
            , hasUuid(false)
            , uuidChanged(false)
            , configChanged(false)
            , parameter(param)
        {
            memset(uuid, 0, 16);
        }

        Value(const std::vector<char>& defaultValue, IParameter& param)
            : datatype(DATATYPE_CUSTOMTYPE)
            , defaultValue(defaultValue)
            , hasUuid(false)
            , uuidChanged(false)
            , configChanged(false)
            , parameter(param)
        {
            memset(uuid, 0, 16);
        }

        void writeMandatory(Writer& out) {
            out.write(static_cast<char>(datatype));
            out.write(static_cast<int32_t>(size));
        }

        void write(Writer& out, bool all) {

            writeMandatory(out);

            // write default value
            if (!defaultValue.empty()) {

                if (all || defaultValueChanged) {
                    out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT));
                    out.write(defaultValue.data(), defaultValue.size());

                    if (!all) {
                        defaultValueChanged = false;
                    }
                }
            } else if (defaultValueChanged) {

                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT));
                out.write(static_cast<int32_t>(0));

                defaultValueChanged = false;
            }

            // uuid
            if (hasUuid) {

                if (all || uuidChanged) {
                    out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_UUID));

                    // TODO
                    out.write(uuid, 16);

                    if (!all) {
                        uuidChanged = false;
                    }
                }

            } else if (uuidChanged) {

                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_UUID));

                // if uuid was cleared we write 0-bytes
                out.write(uuid, 16);
                uuidChanged = false;
            }


            // config
            if (config.size() > 0) {

                if (all || configChanged) {
                    out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_CONFIG));
                    out.write(static_cast<uint32_t>(config.size()));

                    // write
                    out.write((char*)config.data(), config.size());

                    if (!all) {
                        configChanged = false;
                    }
                }

            } else if (configChanged) {

                out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_CONFIG));
                out.write(static_cast<uint32_t>(0));
                configChanged = false;
            }
        }

        // mandatory
        datatype_t datatype;
        uint32_t size;

        // options - base
        std::vector<char> defaultValue;
        bool defaultValueChanged;

        char uuid[16];
        bool hasUuid;
        bool uuidChanged;

        std::vector<char> config;
        bool configChanged;

        IParameter& parameter;
    };
    std::shared_ptr<Value> obj;
    TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}

};

}

#endif // TYPE_CUSTOM_H
