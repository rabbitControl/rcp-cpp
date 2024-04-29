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

#ifndef ENUMTYPE_H
#define ENUMTYPE_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "typedefinition.h"
#include "iparameter.h"
#include "stream_tools.h"

namespace rcp {

typedef TypeDefinition<TinyString, DATATYPE_ENUM, td_enum > EnumTypeDefinition;

template<>
class TypeDefinition<TinyString, DATATYPE_ENUM, td_enum>
    : public IDefaultDefinition<TinyString>
{
public:
    TypeDefinition(EnumTypeDefinition& v)  :
        obj(v.obj)
    {}
    TypeDefinition(const EnumTypeDefinition& v)  :
        obj(v.obj)
    {}

    TypeDefinition(IParameter& param) :
        obj(std::make_shared<Value>(param))
    {}
    TypeDefinition(const std::string& d, IParameter& param) :
        obj(std::make_shared<Value>(d, param))
    {}


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

        return obj->defaultValue.changed()
               || obj->optionsChanged
               || obj->multiselect.changed();
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

        std::cout << "--- type enum ---\n";

        if (hasDefault()) {
            std::cout << "\tdefault: " << getDefault() << "\n";
        }

        if (hasMultiselect()) {
            std::cout << "\tmultiselect: " << getMultiselect() << "\n";
        }

        if (hasOptions()) {
            for (auto& o : obj->options) {
                std::cout << "\toption: " << o << "\n";
            }
        }
    }


    //------------------------------------
    // IDefaultDefinition

    TinyString readValue(std::istream& is) override
    {
        return readTinyString(is);
    }

    //--------
    // default
    const TinyString getDefault() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->defaultValue.value();
    }
    void setDefault(const TinyString& defaultValue) override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue = defaultValue;
        if (obj->defaultValue.changed())
        {
            setDirty();
        }
    }
    bool hasDefault() const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->defaultValue.hasValue();
    }
    void clearDefault() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue.clearValue();
        if (obj->defaultValue.changed())
        {
            setDirty();
        }
    }

    //--------
    // options
    std::vector<std::string>& getOptions() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->options;
    }
    const std::string& getOption(const std::string& selection) const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        auto it = std::find(obj->options.begin(), obj->options.end(), selection);
        if (it != obj->options.end()) {
            return *it;
        }

        return empty_string;
    }
    void addOption(const std::string& option)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->options.push_back(option);
        obj->optionsChanged = true;
        setDirty();
    }
    void setOptions(const std::vector<std::string>& options)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->options = options;
        obj->optionsChanged = true;
        setDirty();
    }
    bool hasOptions() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->options.size() > 0;
    }
    void clearOptions()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->options.clear();
        obj->optionsChanged = true;
        setDirty();
    }

    //------------
    // multiselect
    bool getMultiselect() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        if (obj->multiselect.hasValue())
        {
            return obj->multiselect.value();
        }
        return false;
    }
    bool hasMultiselect() const
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->multiselect.hasValue();
    }
    void setMultiselect(bool value)
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->multiselect = value;
        if (obj->multiselect.changed())
        {
            setDirty();
        }
    }
    void clearMultiselect()
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->multiselect.clearValue();
        if (obj->multiselect.changed())
        {
            setDirty();
        }
    }


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
            enum_options_t opt = static_cast<enum_options_t>(is.get());

            if (is.eof()) {
                break;
            }

            if (opt == TERMINATOR) {
                break;
            }

            switch (opt) {
            case ENUM_OPTIONS_DEFAULT: {

                std::string d = readTinyString(is);
                CHECK_STREAM

                        obj->defaultValue = d;
                break;
            }
            case ENUM_OPTIONS_ENTRIES: {

                obj->options.clear();
                std::string option = readTinyString(is);
                CHECK_STREAM
                    while (option != "") {

                    obj->options.push_back(option);

                    option = readTinyString(is);
                    CHECK_STREAM
                }
                break;
            }
            case ENUM_OPTIONS_MULTISELECT: {

                bool multi = readFromStream(is, multi);
                CHECK_STREAM

                        obj->multiselect = multi;
                break;
            }
            }
        }
    }

    void setAllUnchanged() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        obj->defaultValue.setUnchanged();
        obj->optionsChanged = false;
        obj->multiselect.setUnchanged();
    }

private:
    void setDirty() {
        obj->parameter.setDirty();
    }

    class Value {
    public:
        Value(IParameter& param) : datatype(DATATYPE_ENUM)
            , defaultValue(empty_string)
            , optionsChanged(false)
            , parameter(param)
        {}

        Value(const std::string& defaultValue, IParameter& param) : datatype(DATATYPE_ENUM)
            , defaultValue(defaultValue)
            , optionsChanged(false)
            , parameter(param)
        {}

        void writeMandatory(Writer& out) {
            out.write(static_cast<char>(datatype));
        }

        void write(Writer& out, bool all) {

            writeMandatory(out);

            // write default value
            if (defaultValue.hasValue()) {

                if (all || defaultValue.changed()) {
                    out.write(static_cast<char>(ENUM_OPTIONS_DEFAULT));
                    out.writeTinyString(defaultValue.value());

                    if (!all) {
                        defaultValue.setUnchanged();
                    }
                }
            } else if (defaultValue.changed()) {

                out.write(static_cast<char>(ENUM_OPTIONS_DEFAULT));
                out.writeTinyString("");
                defaultValue.setUnchanged();
            }

            // options
            if (options.size() > 0) {

                if (all || optionsChanged) {
                    out.write(static_cast<char>(ENUM_OPTIONS_ENTRIES));
                    for (const std::string& entry : options) {
                        out.writeTinyString(entry);
                    }
                    out.write(static_cast<uint8_t>(TERMINATOR));

                    if (!all) {
                        optionsChanged = false;
                    }
                }

            } else if (optionsChanged) {

                out.write(static_cast<char>(ENUM_OPTIONS_ENTRIES));
                out.write(static_cast<uint8_t>(TERMINATOR));
                optionsChanged = false;
            }


            // multiselect
            if (multiselect.hasValue()) {

                if (all || multiselect.changed()) {
                    out.write(static_cast<char>(ENUM_OPTIONS_MULTISELECT));
                    out.write(multiselect.value());

                    if (!all) {
                        multiselect.setUnchanged();
                    }
                }

            } else if (multiselect.changed()) {

                out.write(static_cast<char>(ENUM_OPTIONS_MULTISELECT));
                out.write(false);
                multiselect.setUnchanged();
            }
        }

        // mandatory
        datatype_t datatype;

        // options - base
        Option<TinyString> defaultValue;

        // options - enum
        std::vector<std::string> options;
        bool optionsChanged;

        Option<bool> multiselect;

        IParameter& parameter;
    };
    std::shared_ptr<Value> obj;
    TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}
};

}
#endif // ENUMTYPE_H
