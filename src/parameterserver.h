/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2023
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
    GroupParameterPtr getRoot() const { return parameterManager->rootGroup(); }

    ParameterPtr getParameter(const short& id) {
        return parameterManager->getParameter(id);
    }

    //
    void removeParameter(IParameter& parameter) {
        parameterManager->removeParameter(parameter);
    }
    void removeParameter(short id) {
        parameterManager->removeParameter(id);
    }

    // parameter creater
    BooleanParameterPtr createBooleanParameter(const std::string& label) {
        return parameterManager->createBooleanParameter(label);
    }
    BooleanParameterPtr createBooleanParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createBooleanParameter(label, group);
    }

    Int8ParameterPtr createInt8Parameter(const std::string& label) {
        return parameterManager->createInt8Parameter(label, nullptr);
    }
    Int8ParameterPtr createInt8Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt8Parameter(label, group);
    }

    Int16ParameterPtr createInt16Parameter(const std::string& label) {
        return parameterManager->createInt16Parameter(label, nullptr);
    }
    Int16ParameterPtr createInt16Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt16Parameter(label, group);
    }

    Int32ParameterPtr createInt32Parameter(const std::string& label) {
        return parameterManager->createInt32Parameter(label, nullptr);
    }
    Int32ParameterPtr createInt32Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt32Parameter(label, group);
    }

    Int64ParameterPtr createInt64Parameter(const std::string& label) {
        return parameterManager->createInt64Parameter(label, nullptr);
    }
    Int64ParameterPtr createInt64Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt64Parameter(label, group);
    }

    Float32ParameterPtr createFloat32Parameter(const std::string& label) {
        return parameterManager->createFloat32Parameter(label, nullptr);
    }
    Float32ParameterPtr createFloat32Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createFloat32Parameter(label, group);
    }

    Float64ParameterPtr createFloat64Parameter(const std::string& label) {
        return parameterManager->createFloat64Parameter(label, nullptr);
    }
    Float64ParameterPtr createFloat64Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createFloat64Parameter(label, group);
    }


    StringParameterPtr createStringParameter(const std::string& label) {
        return parameterManager->createStringParameter(label, nullptr);
    }
    StringParameterPtr createStringParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createStringParameter(label, group);
    }


    RGBAParameterPtr createRGBAParameter(const std::string& label) {
        return parameterManager->createRGBAParameter(label, nullptr);
    }
    RGBAParameterPtr createRGBAParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createRGBAParameter(label, group);
    }



    BangParameterPtr createBangParameter(const std::string& label) {
        return parameterManager->createBangParameter(label, nullptr);
    }
    BangParameterPtr createBangParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createBangParameter(label, group);
    }

    GroupParameterPtr createGroupParameter(const std::string& label) {
        return parameterManager->createGroupParameter(label, nullptr);
    }
    GroupParameterPtr createGroupParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createGroupParameter(label, group);
    }

    void addParsingErrorCb(ParsingErrorListener* c, void(ParsingErrorListener::* func)()) {
        parsing_error_cb[c] = func;
    }
    void removeParsingErrorCb(ParsingErrorListener* c) {
        parsing_error_cb.erase(c);
    }


    void dumpHierarchy() const {
        parameterManager->dumpHierarchy();
    }

    void setApplicationId(const std::string& appid) {
        m_applicationId = appid;
    }

    std::string getApplicationId() {
        return m_applicationId;
    }

protected:
	std::shared_ptr<ParameterManager> parameterManager;
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
