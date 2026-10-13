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

#ifndef RCP_V10_PARAMETERSERVER_H
#define RCP_V10_PARAMETERSERVER_H

#include <map>
#include <unordered_set>

#include "../ParameterServer.h"

#include "ParameterClientState.h"
#include "GroupParameter.h"
#include "InfoData.h"
#include "ParameterDefinitions.h"

namespace rcp {
namespace v10 {

class ParameterServer
    : public rcp::ParameterServer
{
public:
    ParameterServer();
    ParameterServer(ServerTransporter& transporter);

    void setApplicationVersion(const std::string& version);
    std::string applicationVersion() const;


    BooleanParameterPtr createBooleanParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);

    Int8ParameterPtr createInt8Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    UInt8ParameterPtr createUInt8Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    Int16ParameterPtr createInt16Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    UInt16ParameterPtr createUInt16Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    Int32ParameterPtr createInt32Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    UInt32ParameterPtr createUInt32Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    Int64ParameterPtr createInt64Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    UInt64ParameterPtr createUInt64Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    Float32ParameterPtr createFloat32Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    Float64ParameterPtr createFloat64Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);

    StringParameterPtr createStringParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    EnumParameterPtr createEnumParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    RGBParameterPtr createRGBParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    RGBAParameterPtr createRGBAParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    RGBFloatParameterPtr createRGBFloatParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    RGBAFloatParameterPtr createRGBAFloatParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    URIParameterPtr createURIParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    IPv4ParameterPtr createIPv4Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    IPv6ParameterPtr createIPv6Parameter(const std::string& label, const IGroupParameterPtr& group = nullptr);

    BangParameterPtr createBangParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);
    GroupParameterPtr createGroupParameter(const std::string& label, const IGroupParameterPtr& group = nullptr);


    template<typename T>
    RangeParameterPtr<T> createRangeParameter(const std::string &label, const IGroupParameterPtr &group)
    {
        int16_t id = _getNextId();
        if (id != 0)
        {
            auto p = RangeParameter<T>::create(id);
            _addParameterDirect(label, p, group);

            return p;
        }

        throw std::runtime_error("no valid id...");
    }

public:
    // ParameterServer
    bool update() override;


public:
    // ServerTransporterReceiver
    virtual void received(std::istream& data, ServerTransporter& transporter, void* id) override;
    virtual void connected(void* client) override;
    virtual void disconnected(void* client) override;


private:
    void _init(const std::vector<int32_t>& idData, ServerTransporter& transporter, void *id);
    void _version(const InfoData& info, ServerTransporter& transporter, void *id);
    void _sendVersion(ServerTransporter& transporter, void *id);
    void _sendParameterFull(const IParameterPtr& parameter, ServerTransporter& transporter, void *id);

    void _update(const std::vector<IParameterPtr>&);

    void setState(void* client, ClientState state);

    uint32_t _getNextId()
    {
        for (uint32_t i=1; i<RcpInt::MAX_VALUE; i++)
        {
            if (ids.find(static_cast<int16_t>(i)) == ids.end())
            {
                uint32_t i_s = static_cast<uint32_t>(i);
                ids.insert(i_s);
                return i_s;
            }
        }

        // return invalid id 0
        return 0;
    }

    std::unordered_set<uint32_t> ids;

    RcpVersion m_rcpVersion{1, 0};
    RcpVersion m_handshakeVersion{1, 0};
    bool m_isStrict{false};

    std::map<void*, ClientState> m_clientStates;

    std::string m_applicationVersion;
};

}
}


#endif // PARAMETERSERVER_H
