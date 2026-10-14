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

#ifndef RCP_V10_RCPLANGUAGESTRING_H
#define RCP_V10_RCPLANGUAGESTRING_H

#include <istream>
#include <map>
#include <optional>

#include "RcpString.h"
#include "types.h"

namespace rcp {
namespace v10 {


class RcpLanguageString
    : public rcp::Writeable
{
public:
    static RcpLanguageString parse(std::istream& is)
    {
        std::optional<std::string> anyString;
        std::map<std::string, std::string> languageMap;

        // peek one
        int peek = is.peek();

        // NOTE: check EOF?

        char code[4];
        code[3] = 0;
        while (peek > 0)
        {
            is.read(code, 3);

            std::string code_str(code);
            std::string label = RcpString::parse(is);

            if (code_str == language_string_any)
            {
                anyString = label;
            }
            else
            {
                languageMap[code_str] = label;
            }

            // peek next byte
            peek = is.peek();
        }

        // read 0-byte off the stream
        is.get();


        RcpLanguageString s;

        if (anyString.has_value())
        {
            s.setAnyString(anyString.value());
        }

        s.setLanguageMap(languageMap);

        return s;
    }


public:
    RcpLanguageString()
    {}

    RcpLanguageString(const std::string& anystr,
                      const std::map<std::string, std::string>& languageMap)
        : m_anyString(anystr)
        , m_languageMap(languageMap)
    {}


    std::string anyString() const
    {
        return m_anyString.value_or("");
    }

    void setAnyString(const std::string& str)
    {
        m_anyString = str;
    }

    bool hasAnyString() const
    {
        return m_anyString.has_value();
    }

    const std::map<std::string, std::string>& languageMap() const
    {
        return m_languageMap;
    }

    void setLanguageMap(const std::map<std::string, std::string>& map)
    {
        m_languageMap = map;
    }


    bool operator==(const RcpLanguageString& other) const {
        return m_anyString == other.m_anyString && m_languageMap == other.m_languageMap;
    }

    bool operator!=(const RcpLanguageString& other) const {
        return m_anyString != other.m_anyString || m_languageMap != other.m_languageMap;
    }


public:
    // Writeable interface
    void write(Writer &out, bool all)
    {
        if (m_anyString.has_value())
        {
            out.write(language_string_any, false);
            RcpString(m_anyString.value()).write(out, true);
        }

        for (auto& l : m_languageMap)
        {
            if (l.first.length() < 3)
            {
                continue;
            }

            out.write(l.first.substr(0, 3), false);
            RcpString(l.second).write(out, true);
        }

        out.write(static_cast<char>(0));
    }

private:
    std::optional<std::string> m_anyString;
    std::map<std::string, std::string> m_languageMap;
};

}
}

#endif // RCPLANGUAGESTRING_H
