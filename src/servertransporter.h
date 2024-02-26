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

#ifndef SERVERTRANSPORTER_H
#define SERVERTRANSPORTER_H

#include <map>
#include <istream>

namespace rcp {

    class ServerTransporter;

    class ServerTransporterReceiver
    {
    public:
        virtual void received(std::istream& data, ServerTransporter& transporter, void* id) = 0;
    };


    class ServerTransporter
    {
    public:
        virtual void bind(int port) = 0;
        virtual void unbind() = 0;

        virtual void sendToOne(std::istream& data, void* id) = 0;
        virtual void sendToAll(std::istream& data, void* excludeId) = 0;

        virtual int getConnectionCount() = 0;

        //
        void addReceivedCb(ServerTransporterReceiver* c, void(ServerTransporterReceiver::* func)(std::istream&, ServerTransporter&, void*)) {
            receive_cb[c] = func;
        }
        void removeReceivedCb(ServerTransporterReceiver* c) {
            receive_cb.erase(c);
        }

    protected:
        void _received(std::istream& in, void* client) {
            for (const auto& kv : receive_cb) {
                (kv.first->*kv.second)(in, *this, client);
            }
        }

        std::map<ServerTransporterReceiver*, void(ServerTransporterReceiver::*)(std::istream&, ServerTransporter&, void*)> receive_cb;
    };
}


#endif // SERVERTRANSPORTER_H
