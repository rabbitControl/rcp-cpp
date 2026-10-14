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

#ifndef RCP_V010_STRINGTYPEDEFINITION_H
#define RCP_V010_STRINGTYPEDEFINITION_H

#include "../TypeDefinitionString.h"

#include "ParameterParser.h"
#include "types.h"


namespace rcp {
namespace v010 {

class TypeDefinitionString
    : public rcp::TypeDefinitionString<v010::DATATYPE_STRING, v010::OptionsDefault>
{
public:
    TypeDefinitionString(IParameter* param)
        : rcp::TypeDefinitionString<v010::DATATYPE_STRING, v010::OptionsDefault>(param)
    {}

public:
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
        out.write(this->datatype());

        // write default value
        if (m_defaultValue.hasValue())
        {
            if (all || m_defaultValue.changed())
            {
                out.write(static_cast<char>(STRING_OPTIONS_DEFAULT));

                writeValue(out, m_defaultValue.value());

                if (!all)
                {
                    m_defaultValue.setUnchanged();
                }
            }
        }
        else if (m_defaultValue.changed())
        {
            out.write(static_cast<char>(STRING_OPTIONS_DEFAULT));
            writeValue(out, "");
            m_defaultValue.setUnchanged();
        }

        // regex
        if (m_regex.hasValue())
        {
            if (all || m_regex.changed())
            {
                out.write(static_cast<char>(STRING_OPTIONS_REGULAR_EXPRESSION));
                ParameterWriter::writeString(out, m_regex.value());

                if (!all)
                {
                    m_regex.setUnchanged();
                }
            }

        }
        else if (m_regex.changed())
        {
            out.write(static_cast<char>(STRING_OPTIONS_REGULAR_EXPRESSION));
            ParameterWriter::writeString(out, "");
            m_regex.setUnchanged();
        }


        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }

    virtual bool handleOption(uint8_t optionId, std::istream& is) override
    {
        switch (optionId)
        {
        case STRING_OPTIONS_REGULAR_EXPRESSION: {

            std::string d = ParameterParser::readLongString(is);
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

#endif // STRINGTYPEDEFINITION_H
