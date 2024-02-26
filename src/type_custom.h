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

    template<typename T>
    class TypeDefinition<T, DATATYPE_CUSTOMTYPE, td_custom> : public IDefaultDefinition<T>
    {
    public:
        TypeDefinition(TypeDefinition<std::string, DATATYPE_CUSTOMTYPE, td_custom>& v) :
            obj(v.obj)
        {}

        TypeDefinition(const TypeDefinition<std::string, DATATYPE_CUSTOMTYPE, td_custom>& v) :
            obj(v.obj)
        {}

        TypeDefinition(IParameter& param) :
            obj(std::make_shared<Value>(param))
        {}

        TypeDefinition(const std::string& d, IParameter& param) :
            obj(std::make_shared<Value>(d, param))
        {}


        //------------------------------------
        // Writeable
        void write(Writer& out, bool all) override
        {
            obj->write(out, all);

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }


        //------------------------------------
        // IOptionparser
        void parseOptions(std::istream& is) override
        {
            while (!is.eof()) {

                // read option prefix
                customtype_options_t opt = static_cast<customtype_options_t>(is.get());

                if (is.eof()) {
                    break;
                }

                if (opt == TERMINATOR) {
                    break;
                }

                switch (opt) {
                case CUSTOMTYPE_OPTIONS_DEFAULT: {

                    T def = readFromStream(is, def);
                    CHECK_STREAM

                    obj->defaultValue = def;
                    break;
                }
                case CUSTOMTYPE_OPTIONS_UUID: {

                    char uuid[16];
                    is.get(uuid, 16);
                    CHECK_STREAM

                    obj->hasUuid = true;
                    memcpy(obj->uuid, uuid, 16);
                    break;
                }
                case CUSTOMTYPE_OPTIONS_CONFIG: {

                    uint32_t data_length = readFromStream(is, data_length);
                    CHECK_STREAM

                    char* data = new char[data_length];
                    if (data)
                    {
                        is.get(data, static_cast<std::streamsize>(data_length));
                        CHECK_STREAM

                        //set
                        obj->config.clear();
                        obj->config.resize(data_length);
                        memcpy(&obj->config[0], &data[0], data_length);

                        delete[] data;
                    }

                    break;
                }
                }
            }
        }


        //------------------------------------
        // ITypeDefinition

        datatype_t getDatatype() const override
        {
            return obj->datatype;
        }

        bool anyOptionChanged() const override
        {
            return obj->defaultValue.changed()
                    || obj->uuidChanged
                    || obj->configChanged;
        }

        void writeMandatory(Writer& out) const override
        {
            obj->writeMandatory(out);
        }

        void dump() override
        {
            std::cout << "--- type custom ---\n";

            if (hasDefault()) {
                std::cout << "\tdefault: " << getDefault() << "\n";
            }

            if (hasUuid()) {
                std::cout << "\tuuid: " << getUuid() << "\n";
            }

            if (hasConfig()) {
                std::cout << "\tconfig bytes: " << getConfig().size() << "\n";
            }
        }


        //------------------------------------
        // IDefaultDefinition<T>

        T readValue(std::istream& is) override
        {
            T value = readFromStream(is, value);
            return value;
        }

        //--------
        // default
        const T getDefault() const override
        {
            return obj->defaultValue.value();
        }
        void setDefault(const T& defaultValue) override
        {
            obj->defaultValue = defaultValue;
            if (obj->defaultValue.changed())
            {
                setDirty();
            }
        }
        bool hasDefault() const override
        {
            return obj->defaultValue.hasValue();
        }
        void clearDefault() override
        {
            obj->defaultValue.clearValue();
            if (obj->defaultValue.changed())
            {
                setDirty();
            }
        }

        //------------------------------------
        //

        // uuid
        bool hasUuid() const { return obj->hasUuid; }
        void setUuid(const char* uuid, uint8_t length) {
            if (length != 16) {
                return;
            }

            obj->hasUuid = true;
            if (memcmp(obj->uuid, uuid, 16) == 0) {
                return;
            }

            memcpy(obj->uuid, uuid, 16);
            obj->uuidChanged = true;
            setDirty();
        }
        const char* getUuid() const {
            return obj->uuid;
        }
        void clearUuid() {
            memset(obj->uuid, 0, 16);
            obj->hasUuid = false;
            obj->uuidChanged = true;
            setDirty();
        }


        // config
        bool hasConfig() const
        {
            return obj->config.size() > 0;
        }
        std::vector<int8_t>& getConfig() const
        {
            return obj->config;
        }
        void setConfig(const std::vector<int8_t>& config)
        {
            obj->config = config;
            obj->configChanged = true;
            setDirty();
        }
        void clearConfig() {
            obj->config.clear();
            obj->configChanged = true;
            setDirty();
        }

        void setAllUnchanged() override
        {
            obj->defaultValue.setUnchanged();
            obj->uuidChanged = false;
            obj->configChanged = false;
        }

    private:
        void setDirty() {
            obj->parameter.setDirty();
        }

        class Value {
        public:
            Value(IParameter& param) : datatype(DATATYPE_CUSTOMTYPE)
              , hasUuid(false)
              , uuidChanged(false)
              , configChanged(false)
              , parameter(param)
            {
                memset(uuid, 0, 16);
            }

            Value(const std::string& defaultValue, IParameter& param) : datatype(DATATYPE_CUSTOMTYPE)
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
                out.write(static_cast<int32_t>(sizeof(T)));
            }

            void write(Writer& out, bool all) {

                writeMandatory(out);

                // write default value
                if (defaultValue.hasValue()) {

                    if (all || defaultValue.changed()) {
                        out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT));
                        out.write((char*)(&defaultValue), sizeof(T));

                        if (!all) {
                            defaultValue.setUnchanged();
                        }
                    }
                } else if (defaultValue.changed()) {

                    out.write(static_cast<char>(CUSTOMTYPE_OPTIONS_DEFAULT));

                    T v;
                    memset(&v, 0, sizeof(T));
                    out.write((char*)(&v), sizeof(T));

                    defaultValue.setUnchanged();
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

            // options - base
            Option<T> defaultValue;

            char uuid[16];
            bool hasUuid;
            bool uuidChanged;

            std::vector<int8_t> config;
            bool configChanged;

            IParameter& parameter;
        };
        std::shared_ptr<Value> obj;
        TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}

    };

}

#endif // TYPE_CUSTOM_H
