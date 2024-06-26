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

#ifndef DEFAULTTYPE_H
#define DEFAULTTYPE_H

#include <iostream>

#include "typedefinition.h"
#include "iparameter.h"
#include "stream_tools.h"

namespace rcp {

template<
    typename T,
    datatype_t type_id
    >
class TypeDefinition<T, type_id, td_default>
    : public IDefaultDefinition<T>
{
public:
    TypeDefinition(TypeDefinition<T, type_id, td_default>& v) :
        obj(v.obj)
    {}

    TypeDefinition(const TypeDefinition<T, type_id, td_default>& v) :
        obj(v.obj)
    {}

    TypeDefinition(IParameter& param) :
        obj(std::make_shared<Value>(param))
    {}

    TypeDefinition(const T& defaultValue, IParameter& param) :
        obj(std::make_shared<Value>(defaultValue, param))
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
        return obj->defaultValue.changed();
    }

    void dump() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif

        std::cout << "--- type default ---\n";

        if (hasDefault()) {
            std::cout << "\tdefault: " << getDefault() << "\n";
        }
    }

    void writeMandatory(Writer& out) const override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        obj->writeMandatory(out);
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
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        return obj->defaultValue.value();
    }
    void setDefault(const T& defaultValue) override
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

        while (!is.eof()) {

            // read option prefix
            boolean_options_t opt = static_cast<boolean_options_t>(is.get());

            if (opt == TERMINATOR) {
                break;
            }

            // check stream
            CHECK_STREAM_MSG("typedefinition - could not read from stream")


            switch (opt) {
            case OPTIONS_DEFAULT: {

                T def = readFromStream(is, def);
                CHECK_STREAM

                        obj->defaultValue = def;
                break;
            }
            }

        }
    } // parseOptions

    void setAllUnchanged() override
    {
#ifndef RCP_PARAMETER_NO_LOCKING
        std::lock_guard<std::recursive_mutex> locker(obj->parameter.mutex());
#endif
        obj->defaultValue.setUnchanged();
    }

private:
    void setDirty() {
        obj->parameter.setDirty();
    }

    class Value {
    public:
        Value(IParameter& param) :
            datatype(type_id)
            , parameter(param)
        {
        }

        Value(const T& defaultValue, IParameter& param) :
            datatype(type_id)
            , defaultValue(defaultValue)
            , parameter(param)
        {}

        void writeMandatory(Writer& out) {
            out.write(static_cast<char>(datatype));
        }

        void write(Writer& out, bool all) {

            writeMandatory(out);

            if (defaultValue.hasValue()) {

                if (all || defaultValue.changed()) {
                    out.write(static_cast<char>(OPTIONS_DEFAULT));
                    out.write(defaultValue.value());

                    if (!all) {
                        defaultValue.setUnchanged();
                    }
                }
            } else if (defaultValue.changed()) {

                out.write(static_cast<char>(OPTIONS_DEFAULT));

                T v{};
                out.write(v);

                defaultValue.setUnchanged();
            }
        }

        // mandatory
        datatype_t datatype;

        // options - default
        Option<T> defaultValue;

        IParameter& parameter;
    };
    std::shared_ptr<Value> obj;
    TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}
};


//
typedef TypeDefinition<bool, DATATYPE_BOOLEAN, td_default> BooleanTypeDefinition;
typedef TypeDefinition<Color, DATATYPE_RGBA, td_default > RGBATypeDefinition;
typedef TypeDefinition<Color, DATATYPE_RGB, td_default > RGBTypeDefinition;
typedef TypeDefinition<IPv4, DATATYPE_IPV4, td_default> IPv4TypeDefinition;
typedef TypeDefinition<IPv6, DATATYPE_IPV6, td_default> IPv6TypeDefinition;
}

#endif // DEFAULTTYPE_H
