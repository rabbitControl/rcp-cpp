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

#ifndef TYPE_STRING_H
#define TYPE_STRING_H

#include <iostream>

#include "typedefinition.h"
#include "iparameter.h"
#include "stream_tools.h"

namespace rcp {

    typedef TypeDefinition<std::string, DATATYPE_STRING, td_string > StringTypeDefinition;

    template<>
    class TypeDefinition<std::string, DATATYPE_STRING, td_string> : public IDefaultDefinition<std::string>
    {
    public:
        TypeDefinition(StringTypeDefinition& v)  :
            obj(v.obj)
        {}

        TypeDefinition(const StringTypeDefinition& v) :
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
                string_options_t opt = static_cast<string_options_t>(is.get());

                if (is.eof()) {
                    break;
                }

                if (opt == TERMINATOR) {
                    break;
                }

                switch (opt) {
                case STRING_OPTIONS_DEFAULT: {

                    std::string d = readLongString(is);
                    CHECK_STREAM

                    obj->defaultValue = d;
                    break;
                }
                case STRING_OPTIONS_REGULAR_EXPRESSION: {

                    std::string d = readLongString(is);
                    CHECK_STREAM

                    obj->regex = d;
                    break;
                }
                }
            }
        }


        //------------------------------------
        // ITypeDefinition
        datatype_t getDatatype() const override {
            return obj->datatype;
        }

        void writeMandatory(Writer& out) const override {
            obj->writeMandatory(out);
        }

        bool anyOptionChanged() const override
        {
            return obj->defaultValue.changed()
                    || obj->regex.changed();
        }

        void dump() override
        {
            std::cout << "--- type string ---\n";

            if (hasDefault()) {
                std::cout << "\tdefault: " << getDefault() << "\n";
            }

            if (hasRegex()) {
                std::cout << "\tdefault: " << getRegex() << "\n";
            }
        }

        //------------------------------------
        // IDefaultDefinition

        std::string readValue(std::istream& is) override
        {
            return readLongString(is);
        }

        //----------------------
        // default
        const std::string getDefault() const override
        {
            return obj->defaultValue.value();
        }
        void setDefault(const std::string& defaultValue) override
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



        //----------------------
        // regex
        virtual const std::string& getRegex() const {
            return obj->regex.value();
        }
        virtual void setRegex(const std::string& value) {
            obj->regex = value;
            if (obj->regex.changed())
            {
                setDirty();
            }
        }
        virtual bool hasRegex() const {
            return obj->regex.hasValue();
        }
        virtual void clearRegex() {
            obj->regex.clearValue();
            if (obj->regex.changed())
            {
                setDirty();
            }
        }


        void setAllUnchanged() override
        {
            obj->defaultValue.setUnchanged();
            obj->regex.setUnchanged();
        }

    private:
        void setDirty() {
            obj->parameter.setDirty();
        }

        class Value {
        public:
            Value(IParameter& param) : datatype(DATATYPE_STRING)
              , defaultValue("")
              , parameter(param)
            {}

            Value(const std::string& defaultValue, IParameter& param) : datatype(DATATYPE_STRING)
              , defaultValue(defaultValue)
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
                        out.write(static_cast<char>(STRING_OPTIONS_DEFAULT));
                        out.writeString(defaultValue.value());

                        if (!all) {
                            defaultValue.setUnchanged();
                        }
                    }
                } else if (defaultValue.changed()) {

                    out.write(static_cast<char>(STRING_OPTIONS_DEFAULT));
                    out.writeString("");
                    defaultValue.setUnchanged();
                }

                // regex
                if (regex.hasValue()) {

                    if (all || regex.changed()) {
                        out.write(static_cast<char>(STRING_OPTIONS_REGULAR_EXPRESSION));
                        out.writeString(regex.value());

                        if (!all) {
                            regex.setUnchanged();
                        }
                    }

                } else if (regex.changed()) {

                    out.write(static_cast<char>(STRING_OPTIONS_REGULAR_EXPRESSION));
                    out.writeString("");
                    regex.setUnchanged();
                }

            }

            // mandatory
            datatype_t datatype;

            // options - base
            Option<std::string> defaultValue;
            // options - regex
            Option<std::string> regex;

            IParameter& parameter;
        };
        std::shared_ptr<Value> obj;
        TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}
    };

}
#endif // TYPE_STRING_H
