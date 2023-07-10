/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2023
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef URITYPE_H
#define URITYPE_H

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

#include "typedefinition.h"
#include "iparameter.h"
#include "stream_tools.h"

namespace rcp {

    typedef TypeDefinition<std::string, DATATYPE_URI, td_uri> UriTypeDefinition;

    template<>
    class TypeDefinition<std::string, DATATYPE_URI, td_uri> : public IDefaultDefinition<std::string>
    {
    public:
        TypeDefinition(UriTypeDefinition& v) :
            obj(v.obj)
        {}

        TypeDefinition(const UriTypeDefinition& v) :
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
            return obj->datatype;
        }

        void writeMandatory(Writer& out) const override
        {
            obj->writeMandatory(out);
        }

        bool anyOptionChanged() const override
        {
            return obj->defaultValue.changed()
                    || obj->filter.changed()
                    || obj->schemaChanged;
        }

        void dump() override
        {
            std::cout << "--- type uri ---\n";

            if (hasDefault()) {
                std::cout << "\tdefault: " << getDefault() << "\n";
            }

            if (hasFilter()) {
                std::cout << "\tdefault: " << getFilter() << "\n";
            }

            if (obj->schemas.size() > 0) {
                for (auto& s : obj->schemas) {
                    std::cout << "\tschema: " << s << "\n";
                }
            }
        }

        //------------------------------------
        // IDefaultDefinition
        std::string readValue(std::istream& is) override
        {
            return readLongString(is);
        }

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
        // options - filter
        std::string getFilter() const {
            return obj->filter.value();
        }
        bool hasFilter() const {
            return obj->filter.hasValue();
        }
        void setFilter(const std::string& filter) {
            obj->filter = filter;
            if (obj->filter.changed())
            {
                setDirty();
            }
        }
        void clearFilter() {
            obj->filter.clearValue();
            if (obj->filter.changed())
            {
                setDirty();
            }
        }

        //----------------------
        // options - schemas
        std::vector<std::string> getSchemas() const {
            return obj->schemas;
        }
        void addSchema(const std::string& schema) {

            if (std::find(obj->schemas.begin(), obj->schemas.end(), schema) != obj->schemas.end())
            {
                return;
            }

            obj->schemas.push_back(schema);
            obj->schemaChanged = true;
            setDirty();
        }
        void setSchemas(const std::vector<std::string>& schemas) {
            obj->schemas = schemas;
            obj->schemaChanged = true;
            setDirty();
        }
        void clearSchemas() {
            obj->schemaChanged = !obj->schemas.empty();
            obj->schemas.clear();
            if (obj->schemaChanged)
            {
                setDirty();
            }
        }



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
                uri_options_t opt = static_cast<uri_options_t>(is.get());

                if (is.eof()) {
                    break;
                }

                if (opt == TERMINATOR) {
                    break;
                }

                switch (opt) {
                case URI_OPTIONS_DEFAULT: {

                    std::string def = readLongString(is);
                    CHECK_STREAM

                    obj->defaultValue = def;
                    break;
                }
                case URI_OPTIONS_FILTER: {

                    std::string filter = readTinyString(is);
                    CHECK_STREAM

                    obj->filter = filter;
                    break;
                }

                case URI_OPTIONS_SCHEMA: {
                    std::string schemas_str = readTinyString(is);
                    CHECK_STREAM

                    obj->schemas.clear();
                    std::stringstream strstr(schemas_str);
                    std::istream_iterator<std::string> begin(strstr);
                    std::istream_iterator<std::string> end;
                    obj->schemas.assign(begin, end);
                    break;
                }

                }
            }
        }


        void setAllUnchanged() override
        {
            obj->defaultValue.setUnchanged();
            obj->filter.setUnchanged();
            obj->schemaChanged = false;;
        }

    private:
        void setDirty() {
            obj->parameter.setDirty();
        }

        class Value {
        public:
            Value(IParameter& param) : datatype(DATATYPE_URI)
              , schemaChanged(false)
              , parameter(param)
            {}

            Value(const std::string& defaultValue, IParameter& param) : datatype(DATATYPE_URI)
              , defaultValue(defaultValue)
              , schemaChanged(false)
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
                        out.write(static_cast<char>(URI_OPTIONS_DEFAULT));
                        out.writeString(defaultValue.value());

                        if (!all) {
                            defaultValue.setUnchanged();
                        }
                    }
                } else if (defaultValue.changed()) {

                    out.write(static_cast<char>(URI_OPTIONS_DEFAULT));
                    out.writeString("");
                    defaultValue.setUnchanged();
                }

                // filter
                if (filter.hasValue()) {

                    if (all || filter.changed()) {
                        out.write(static_cast<char>(URI_OPTIONS_FILTER));
                        out.writeTinyString(filter.value());

                        if (!all) {
                            filter.setUnchanged();
                        }
                    }
                } else if (filter.changed()) {

                    out.write(static_cast<char>(URI_OPTIONS_FILTER));
                    out.writeTinyString("");
                    filter.setUnchanged();
                }

                // schemas
                if (schemas.size() > 0) {

                    if (all | schemaChanged) {
                        out.write(static_cast<char>(URI_OPTIONS_SCHEMA));

                        std::ostringstream os;
                        std::copy(schemas.begin(), schemas.end()-1, std::ostream_iterator<std::string>(os, " "));
                        os << *schemas.rbegin();

                        out.writeTinyString(os.str());

                        if (!all) {
                            schemaChanged = false;
                        }
                    }
                } else if (schemaChanged) {

                    out.write(static_cast<char>(URI_OPTIONS_SCHEMA));
                    out.writeTinyString("");
                    schemaChanged = false;
                }
            }

            // mandatory
            datatype_t datatype;

            // options - default
            Option<std::string> defaultValue;
            // options - filter
            Option<std::string> filter;

            // options - schema
            std::vector<std::string> schemas;
            bool schemaChanged;

            IParameter& parameter;
        };
        std::shared_ptr<Value> obj;
        TypeDefinition(std::shared_ptr<Value> obj) :obj(obj) {}
    };

}

#endif // URITYPE_H
