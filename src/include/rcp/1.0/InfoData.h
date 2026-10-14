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

#ifndef RCP_V10_INFODATA_H
#define RCP_V10_INFODATA_H

#include "../Writeable.h"
#include "../Option.h"

#include "RcpString.h"
#include "RcpVersion.h"
#include "types.h"

namespace rcp {
namespace v10 {

class InfoData
    : public rcp::Writeable
{
public:
    static InfoData parse(std::istream& is)
    {
        auto version = RcpVersion::parse(is);
        auto handshakeVersion = RcpVersion::parse(is);

        InfoData info(version, handshakeVersion);

        char optionId;
        while (is.get(optionId))
        {
            if (optionId == Terminator)
            {
                break;
            }

            switch((optionId & TerminatorMask))
            {
            case INFODATA_OPTIONS_APPLICATIONID:
                info.setApplicationId(RcpString::parse(is));
                break;

            case INFODATA_OPTIONS_APPLICATIONVERSION:
                info.setApplicationVersion(RcpString::parse(is));
                break;

            default:
                throw std::runtime_error("Unknown option: " + std::to_string(optionId));
            }

            if (optionId & Terminator)
            {
                break;
            }
        }

        return info;
    }

public:
    InfoData(RcpVersion version, RcpVersion handshakeversion)
        : m_protocolVersion(version)
        , m_handshakeVersion(handshakeversion)
    {}

public:
    // Writeable interface
    void write(Writer &out, bool all)
    {
        // write mandatory
        m_protocolVersion.write(out, all);
        m_handshakeVersion.write(out, all);

        if (m_applicationId.hasValue())
        {
            out.write(static_cast<char>(INFODATA_OPTIONS_APPLICATIONID | (m_applicationVersion.hasValue() ? 0x00 : Terminator)));
            RcpString(m_applicationId.value()).write(out, all);
        }

        if (m_applicationVersion.hasValue())
        {
            out.write(static_cast<char>(INFODATA_OPTIONS_APPLICATIONVERSION | Terminator));
            RcpString(m_applicationVersion.value()).write(out, all);
        }

        // write options
        if (!m_applicationId.hasValue() &&
            !m_applicationVersion.hasValue())
        {
            out.write(Terminator);
        }
    }

    RcpVersion protocolVersion() const;
    void setProtocolVersion(RcpVersion version);

    RcpVersion handshakeVersion() const;
    void setHandshakeVersion(RcpVersion handshakeVersion);

    std::string applicationId() const;
    void setApplicationId(const std::string& applicationId);

    std::string applicationVersion() const;
    void setApplicationVersion(const std::string& applicationVersion);

private:
    RcpVersion m_protocolVersion;
    RcpVersion m_handshakeVersion;

    Option<std::string> m_applicationId;
    Option<std::string> m_applicationVersion;
};

inline RcpVersion InfoData::protocolVersion() const
{
    return m_protocolVersion;
}

inline void InfoData::setProtocolVersion(RcpVersion version)
{
    m_protocolVersion = version;
}

inline RcpVersion InfoData::handshakeVersion() const
{
    return m_handshakeVersion;
}

inline void InfoData::setHandshakeVersion(RcpVersion handshakeVersion)
{
    m_handshakeVersion = handshakeVersion;
}

inline std::string InfoData::applicationId() const
{
    return m_applicationId.value();
}

inline void InfoData::setApplicationId(const std::string & applicationId)
{
    m_applicationId = applicationId;
}

inline std::string InfoData::applicationVersion() const
{
    return m_applicationVersion.value();
}

inline void InfoData::setApplicationVersion(const std::string & applicationVersion)
{
    m_applicationVersion = applicationVersion;
}

std::ostream& operator<<(std::ostream& out, const InfoData& info);

}
}

#endif // INFODATA_H
