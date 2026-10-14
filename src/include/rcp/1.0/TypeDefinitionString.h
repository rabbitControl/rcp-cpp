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

#ifndef RCP_V10_TYPEDEFINITIONSTRING_H
#define RCP_V10_TYPEDEFINITIONSTRING_H

#include "../TypeDefinitionString.h"
#include "../IParameter.h"

#include "ParameterParser.h"
#include "ParameterWriter.h"
#include "types.h"

namespace rcp {
namespace v10 {


class TypeDefinitionString
    : public rcp::TypeDefinitionString<DATATYPE_STRING, OptionsDefault>
{
public:
    TypeDefinitionString(rcp::IParameter* param)
        : rcp::TypeDefinitionString<DATATYPE_STRING, OptionsDefault>(param)
    {}

public:
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
        auto write_regex = (all && this->m_regex.hasValue()) || this->m_regex.changed();

        // write datatype
        out.write(static_cast<char>(this->datatype() | (write_default || write_regex ? 0x00 : Terminator)));


        // write default value
        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(STRING_OPTIONS_DEFAULT | (write_regex ? 0x00 : Terminator)));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(STRING_OPTIONS_DEFAULT | (write_regex ? 0x00 : Terminator)));
            writeValue(out, "");
            m_defaultValue.setUnchanged();
        }

        // regex
        if (m_regex.hasValue())
        {
            if (all || m_regex.changed())
            {
                out.write(static_cast<char>(STRING_OPTIONS_REGULAR_EXPRESSION | Terminator));
                ParameterWriter::writeString(out, m_regex.value());

                if (!all)
                {
                    m_regex.setUnchanged();
                }
            }

        }
        else if (m_regex.changed())
        {
            out.write(static_cast<char>(STRING_OPTIONS_REGULAR_EXPRESSION | Terminator));
            ParameterWriter::writeString(out, "");
            m_regex.setUnchanged();
        }
    }

    virtual bool handleOption(uint8_t optionId, std::istream& is) override
    {
        switch (optionId)
        {
        case STRING_OPTIONS_REGULAR_EXPRESSION: {

            std::string d = ParameterParser::readString(is);
            CHECK_STREAM;

            m_regex = d;
            return true;
        }
        }

        return TypeDefinitionDefault::handleOption(optionId, is);
    }
};


}
}

#endif // RCP_V10_TYPEDEFINITIONSTRING_H
