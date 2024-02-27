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

#ifndef CLIENTTRANSPORTER_H
#define CLIENTTRANSPORTER_H

#include <map>

namespace rcp {

class ClientTransporterListener
{
public:
    virtual void connected() = 0;
    virtual void disconnected() = 0;
    virtual void received(std::istream& data) = 0;
};


class ClientTransporter
{
public:

    virtual void connect(std::string host, int port, bool secure = false) = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() = 0;

    virtual void send(std::istream& data) = 0;
    virtual void send(char* data, size_t size) = 0;

    void addConnectedCb(ClientTransporterListener* c, void(ClientTransporterListener::* func)()) {
        connected_cb[c] = func;
    }
    void removeConnectedCb(ClientTransporterListener* c) {
        connected_cb.erase(c);
    }

    void addDisconnectedCb(ClientTransporterListener* c, void(ClientTransporterListener::* func)()) {
        disconnected_cb[c] = func;
    }
    void removeDisconnectedCb(ClientTransporterListener* c) {
        disconnected_cb.erase(c);
    }

    void addReceivedCb(ClientTransporterListener* c, void(ClientTransporterListener::* func)(std::istream&)) {
        receive_cb[c] = func;
    }
    void removeReceivedCb(ClientTransporterListener* c) {
        receive_cb.erase(c);
    }

protected:
    void _connected() {
        for (const auto& kv : connected_cb) {
            (kv.first->*kv.second)();
        }
    }
    void _disconnected() {
        for (const auto& kv : disconnected_cb) {
            (kv.first->*kv.second)();
        }
    }
    void _received(std::istream& in) {
        for (const auto& kv : receive_cb) {
            (kv.first->*kv.second)(in);
        }
    }


    std::map<ClientTransporterListener*, void(ClientTransporterListener::*)()> connected_cb;
    std::map<ClientTransporterListener*, void(ClientTransporterListener::*)()> disconnected_cb;
    std::map<ClientTransporterListener*, void(ClientTransporterListener::*)(std::istream&)> receive_cb;
};

}

#endif // CLIENTTRANSPORTER_H
