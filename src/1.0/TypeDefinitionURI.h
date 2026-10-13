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

#ifndef RCP_V10_URITYPEDEFINITION_H
#define RCP_V10_URITYPEDEFINITION_H

#include <sstream>

#include "../TypeDefinition.h"
#include "../TypeDefinitionURI.h"

#include "RcpString.h"
#include "types.h"

namespace rcp {
namespace v10 {


class TypeDefinitionURI
    : public rcp::TypeDefinitionURI<DATATYPE_URI, OptionsDefault>
{
public:
    TypeDefinitionURI(rcp::IParameter* param)
        : rcp::TypeDefinitionURI<DATATYPE_URI, OptionsDefault>(param)
    {}

    virtual std::string parseValue(std::istream& is) override
    {
        return ParameterParser::readString(is);
    }

    virtual void writeValue(Writer& out, const std::string& value) override
    {
        ParameterWriter::writeString(out, value);
    }

    virtual void write(Writer& out, bool all) override
    {
        auto write_default = (all && this->m_defaultValue.hasValue()) || this->m_defaultValue.changed();
        auto write_filter = (all && this->m_filter.hasValue()) || this->m_filter.changed();
        auto write_schemas = (all && !this->m_schemas.empty()) || this->m_schemaChanged;

        auto all_or = write_default || write_filter || write_schemas;

        out.write(static_cast<char>(this->datatype() | (all_or ? 0x00 : Terminator)));


        all_or = write_filter || write_schemas;

        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(URI_OPTIONS_DEFAULT | (all_or ? 0x00 : Terminator)));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(URI_OPTIONS_DEFAULT | (all_or ? 0x00 : Terminator)));
            writeValue(out, empty_string);
            m_defaultValue.setUnchanged();
        }


        // filter

        all_or = write_schemas;

        if (m_filter.hasValue())
        {
            if (all || m_filter.changed())
            {
                out.write(static_cast<char>(URI_OPTIONS_FILTER | (all_or ? 0x00 : Terminator)));

                ParameterWriter::writeString(out, m_filter.value());

                if (!all)
                {
                    m_filter.setUnchanged();
                }
            }
        }
        else if (m_filter.changed())
        {
            out.write(static_cast<char>(URI_OPTIONS_FILTER | (all_or ? 0x00 : Terminator)));
            ParameterWriter::writeString(out, empty_string);
            m_filter.setUnchanged();
        }



        // schemas
        if (m_schemas.size() > 0)
        {
            if (all | m_schemaChanged)
            {
                out.write(static_cast<char>(URI_OPTIONS_SCHEMA | Terminator));

                std::ostringstream os;
                std::copy(m_schemas.begin(), m_schemas.end()-1, std::ostream_iterator<std::string>(os, " "));
                os << *m_schemas.rbegin();

                ParameterWriter::writeString(out, os.str());

                if (!all)
                {
                    m_schemaChanged = false;
                }
            }
        }
        else if (m_schemaChanged)
        {
            out.write(static_cast<char>(URI_OPTIONS_SCHEMA | Terminator));
            ParameterWriter::writeString(out, empty_string);
            m_schemaChanged = false;
        }
    }


    bool handleOption(uint8_t optionId, std::istream &is) override
    {
        switch (optionId)
        {
        case URI_OPTIONS_DEFAULT:
        {
            std::string def = RcpString::parse(is);
            CHECK_STREAM;

            m_defaultValue = def;

            return true;
        }
        case URI_OPTIONS_FILTER:
        {
            std::string filter = RcpString::parse(is);
            CHECK_STREAM;

            m_filter = filter;

            return true;
        }

        case URI_OPTIONS_SCHEMA:
        {
            std::string schemas_str = RcpString::parse(is);
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
