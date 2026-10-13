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

#ifndef RCP_V10_RCPVERSION_H
#define RCP_V10_RCPVERSION_H

#include "../Writeable.h"
#include "../StreamTools.h"

namespace rcp {
namespace v10 {

class RcpVersion
    : public rcp::Writeable
{
public:
    static RcpVersion parse(std::istream& is)
    {
        RcpVersion version;

        version.m_major = readFromStream(is, int8_t{});
        version.m_minor = readFromStream(is, int8_t{});

        return version;
    }

    static std::string to_string(RcpVersion version)
    {
        return std::to_string(version.m_major) + "." + std::to_string(version.m_minor);
    }

public:
    RcpVersion(){}

    RcpVersion(uint8_t major, uint8_t minor)
        : m_major(major)
        , m_minor(minor)
    {}

    uint8_t major() const
    {
        return m_major;
    }

    uint8_t minor() const
    {
        return m_minor;
    }

    RcpVersion& operator=(const RcpVersion& other)
    {
        m_major = other.m_major;
        m_minor = other.m_minor;
        return *this;
    }

    bool operator==(const RcpVersion& other) const
    {
        return m_major == other.m_major &&
               m_minor == other.m_minor;
    }

    bool operator<(const RcpVersion& other) const
    {
        return m_major < other.m_major || (m_major == other.m_major && m_minor < other.m_minor);
    }
    bool operator<=(const RcpVersion& other) const
    {
        return m_major < other.m_major || (m_major == other.m_major && m_minor <= other.m_minor);
    }

    bool operator>(const RcpVersion& other) const
    {
        return m_major > other.m_major || (m_major == other.m_major && m_minor > other.m_minor);
    }
    bool operator>=(const RcpVersion& other) const
    {
        return m_major > other.m_major || (m_major == other.m_major && m_minor >= other.m_minor);
    }

public:
    // Writeable interface
    void write(Writer &out, bool all)
    {
        out.write(m_major);
        out.write(m_minor);
    }

private:
    uint8_t m_major{0};
    uint8_t m_minor{0};
};

std::ostream& operator<<(std::ostream& out, const RcpVersion& version);

}
}

#endif // RCPVERSION_H
