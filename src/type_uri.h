#ifndef URITYPE_H
#define URITYPE_H

#include <iostream>
#include <sstream>
#include <vector>

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

        ~TypeDefinition();

        //------------------------------------
        // implement ITypeDefinition
        virtual datatype_t getDatatype() const { return obj->datatype; }

        virtual const std::string& getDefault() const { return obj->defaultValue; }
        virtual void setDefault(const std::string& defaultValue) {

            obj->hasDefaultValue = true;

            if (obj->defaultValue == defaultValue) {
                return;
            }

            obj->defaultValue = defaultValue;
            obj->defaultValueChanged = true;
        }
        virtual bool hasDefault() const { return obj->hasDefaultValue; }
        virtual void clearDefault() { obj->hasDefaultValue = false; }

        // options - filter
        std::string getFilter() const { return obj->filter; }
        bool hasFilter() const { return obj->hasFilter; }
        void setFilter(const std::string& filter) {

            obj->hasFilter = true;

            if (obj->filter == filter) {
                return;
            }

            obj->filter = filter;
            obj->filterChanged = true;
            setDirty();
        }
        void clearFilter() {
            obj->hasFilter = false;
            obj->filterChanged = true;
            setDirty();
        }

        // options - schemas
        std::vector<std::string> getSchemas() const { return obj->schemas; }
        void addSchema(const std::string& schema) {
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
            obj->schemas.clear();
            obj->schemaChanged = true;
            setDirty();
        }



        //------------------------------------
        // implement writeable
        void write(Writer& out, bool all) {

            obj->write(out, all);

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }

        //------------------------------------
        // implement optionparser
        void parseOptions(std::istream& is) {

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

                    setDefault(def);
                    break;
                }
                case URI_OPTIONS_FILTER: {

                    std::string filter = readTinyString(is);
                    CHECK_STREAM

                    setFilter(filter);
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

        virtual std::string readValue(std::istream& is) {
            return readLongString(is);
        }


        virtual void dump() {
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

    private:
        void setDirty() {
            obj->parameter.setDirty();
        }

        class Value {
        public:
            Value(IParameter& param) : datatype(DATATYPE_URI)
              , hasDefaultValue(false)
              , defaultValueChanged(false)
              , hasFilter(false)
              , filterChanged(false)
              , schemaChanged(false)
              , parameter(param)
            {}

            Value(const std::string& defaultValue, IParameter& param) : datatype(DATATYPE_URI)
              , defaultValue(defaultValue)
              , hasDefaultValue(true)
              , defaultValueChanged(true)
              , hasFilter(false)
              , filterChanged(false)
              , schemaChanged(false)
              , parameter(param)
            {}

            void write(Writer& out, bool all) {

                out.write(static_cast<char>(datatype));

                // write default value
                if (hasDefaultValue) {

                    if (all || defaultValueChanged) {
                        out.write(static_cast<char>(URI_OPTIONS_DEFAULT));
                        out.writeString(defaultValue);

                        if (!all) {
                            defaultValueChanged = false;
                        }
                    }
                } else if (defaultValueChanged) {

                    out.write(static_cast<char>(URI_OPTIONS_DEFAULT));
                    out.writeString("");
                    defaultValueChanged = false;
                }

                // filter
                if (hasFilter) {

                    if (all || filterChanged) {
                        out.write(static_cast<char>(URI_OPTIONS_FILTER));
                        out.writeTinyString(filter);

                        if (!all) {
                            filterChanged = false;
                        }
                    }
                } else if (filterChanged) {

                    out.write(static_cast<char>(URI_OPTIONS_FILTER));
                    out.writeTinyString("");
                    filterChanged = false;
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
            std::string defaultValue{};
            bool hasDefaultValue;
            bool defaultValueChanged;

            // options - filter
            std::string filter{};
            bool hasFilter;
            bool filterChanged;

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
