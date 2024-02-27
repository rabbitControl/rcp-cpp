/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCPSERVER_H
#define RCPSERVER_H

#include <set>

#include "servertransporter.h"
#include "parametermanager.h"
#include "rcp_error_listener.h"

namespace rcp {

class Packet;

class ParameterServer : public ServerTransporterReceiver
{
public:
    ParameterServer();
    ParameterServer(ServerTransporter& transporter);
    ~ParameterServer();

public:
    void dispose();
    void clear();

    bool addTransporter(ServerTransporter& transporter);
    bool removeTransporter(ServerTransporter& transporter);
    int getConnectionCount();

    virtual bool update();

public:
    // ServerTransporterReceiver
    void received(std::istream& data, ServerTransporter& transporter, void* id);

public:
    GroupParameterPtr getRoot() const { return m_parameterManager->rootGroup(); }

    ParameterPtr getParameter(const short& id) {
        return m_parameterManager->getParameter(id);
    }

    //
    void removeParameter(ParameterPtr parameter) {
        m_parameterManager->removeParameter(parameter);
    }
    void removeParameter(short id) {
        m_parameterManager->removeParameter(id);
    }

    // parameter creation
    BooleanParameterPtr createBooleanParameter(const std::string& label) {
        return m_parameterManager->createBooleanParameter(label);
    }
    BooleanParameterPtr createBooleanParameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createBooleanParameter(label, group);
    }

    Int8ParameterPtr createInt8Parameter(const std::string& label) {
        return m_parameterManager->createInt8Parameter(label, nullptr);
    }
    Int8ParameterPtr createInt8Parameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createInt8Parameter(label, group);
    }

    Int16ParameterPtr createInt16Parameter(const std::string& label) {
        return m_parameterManager->createInt16Parameter(label, nullptr);
    }
    Int16ParameterPtr createInt16Parameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createInt16Parameter(label, group);
    }

    Int32ParameterPtr createInt32Parameter(const std::string& label) {
        return m_parameterManager->createInt32Parameter(label, nullptr);
    }
    Int32ParameterPtr createInt32Parameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createInt32Parameter(label, group);
    }

    Int64ParameterPtr createInt64Parameter(const std::string& label) {
        return m_parameterManager->createInt64Parameter(label, nullptr);
    }
    Int64ParameterPtr createInt64Parameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createInt64Parameter(label, group);
    }

    Float32ParameterPtr createFloat32Parameter(const std::string& label) {
        return m_parameterManager->createFloat32Parameter(label, nullptr);
    }
    Float32ParameterPtr createFloat32Parameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createFloat32Parameter(label, group);
    }

    Float64ParameterPtr createFloat64Parameter(const std::string& label) {
        return m_parameterManager->createFloat64Parameter(label, nullptr);
    }
    Float64ParameterPtr createFloat64Parameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createFloat64Parameter(label, group);
    }


    StringParameterPtr createStringParameter(const std::string& label) {
        return m_parameterManager->createStringParameter(label, nullptr);
    }
    StringParameterPtr createStringParameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createStringParameter(label, group);
    }


    RGBAParameterPtr createRGBAParameter(const std::string& label) {
        return m_parameterManager->createRGBAParameter(label, nullptr);
    }
    RGBAParameterPtr createRGBAParameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createRGBAParameter(label, group);
    }



    BangParameterPtr createBangParameter(const std::string& label) {
        return m_parameterManager->createBangParameter(label, nullptr);
    }
    BangParameterPtr createBangParameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createBangParameter(label, group);
    }

    GroupParameterPtr createGroupParameter(const std::string& label) {
        return m_parameterManager->createGroupParameter(label, nullptr);
    }
    GroupParameterPtr createGroupParameter(const std::string& label, GroupParameterPtr& group) {
        return m_parameterManager->createGroupParameter(label, group);
    }

    void addParsingErrorCb(ParsingErrorListener* c, void(ParsingErrorListener::* func)()) {
        parsing_error_cb[c] = func;
    }
    void removeParsingErrorCb(ParsingErrorListener* c) {
        parsing_error_cb.erase(c);
    }


    void dumpHierarchy() const {
        m_parameterManager->dumpHierarchy();
    }

    void setApplicationId(const std::string& appid) {
        m_applicationId = appid;
    }

    std::string getApplicationId() {
        return m_applicationId;
    }

protected:
    std::shared_ptr<ParameterManager> m_parameterManager;
    std::vector<std::reference_wrapper<ServerTransporter> > transporterList;

private:
    void _init(ServerTransporter& transporter, void *id);
    bool _update(Packet& Packet, ServerTransporter& transporter, void *id);
    void _version(Packet& packet, ServerTransporter& transporter, void *id);
    void _sendParameterFull(ParameterPtr& parameter, ServerTransporter& transporter, void *id);
    void sendPacket(Packet& packet, void *id=nullptr);

    std::string m_applicationId;
    //    Events:
    std::map<ParsingErrorListener*, void(ParsingErrorListener::*)()> parsing_error_cb;
    //    onError(Exception ex);
};

}
#endif // RCPSERVER_H
