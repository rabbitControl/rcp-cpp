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

#ifndef RCP_V010_URITYPEDEFINITION_H
#define RCP_V010_URITYPEDEFINITION_H

#include <sstream>

#include "../TypeDefinition.h"
#include "../TypeDefinitionURI.h"

#include "types.h"

namespace rcp {
namespace v010 {


class TypeDefinitionURI
    : public rcp::TypeDefinitionURI<DATATYPE_URI, OptionsDefault>
{
public:
    TypeDefinitionURI(IParameter* param)
        : rcp::TypeDefinitionURI<DATATYPE_URI, OptionsDefault>(param)
    {}

    virtual std::string parseValue(std::istream& is) override
    {
        return ParameterParser::readLongString(is);
    }

    virtual void writeValue(Writer& out, const std::string& value) override
    {
        ParameterWriter::writeString(out, value);
    }

    virtual void write(Writer& out, bool all) override
    {
        out.write(datatype());


        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(OPTIONS_DEFAULT));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(OPTIONS_DEFAULT));
            writeValue(out, empty_string);
            m_defaultValue.setUnchanged();
        }


        // filter
        if (m_filter.hasValue())
        {
            if (all || m_filter.changed())
            {
                out.write(static_cast<char>(URI_OPTIONS_FILTER));
                ParameterWriter::writeTinyString(out, m_filter.value());

                if (!all)
                {
                    m_filter.setUnchanged();
                }
            }
        }
        else if (m_filter.changed())
        {
            out.write(static_cast<char>(URI_OPTIONS_FILTER));
            ParameterWriter::writeTinyString(out, "");
            m_filter.setUnchanged();
        }

        // schemas
        if (m_schemas.size() > 0)
        {
            if (all | m_schemaChanged)
            {
                out.write(static_cast<char>(URI_OPTIONS_SCHEMA));

                std::ostringstream os;
                std::copy(m_schemas.begin(), m_schemas.end()-1, std::ostream_iterator<std::string>(os, " "));
                os << *m_schemas.rbegin();

                ParameterWriter::writeTinyString(out, os.str());

                if (!all)
                {
                    m_schemaChanged = false;
                }
            }
        }
        else if (m_schemaChanged)
        {
            out.write(static_cast<char>(URI_OPTIONS_SCHEMA));
            ParameterWriter::writeTinyString(out, empty_string);
            m_schemaChanged = false;
        }


        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }


    bool handleOption(uint8_t optionId, std::istream &is) override
    {
        switch (optionId)
        {
        case URI_OPTIONS_DEFAULT:
        {
            std::string def = ParameterParser::readLongString(is);
            CHECK_STREAM;

            m_defaultValue = def;

            return true;
        }
        case URI_OPTIONS_FILTER:
        {
            std::string filter = ParameterParser::readTinyString(is);
            CHECK_STREAM;

            filter = filter;

            return true;
        }

        case URI_OPTIONS_SCHEMA:
        {
            std::string schemas_str = ParameterParser::readTinyString(is);
            CHECK_STREAM;

            m_schemas.clear();

            std::stringstream strstr(schemas_str);
            std::istream_iterator<std::string> begin(strstr);
            std::istream_iterator<std::string> end;

            m_schemas.assign(begin, end);

            return true;
        }
        }

        return false;
    }

};


}
}

#endif // URITYPEDEFINITION_H
