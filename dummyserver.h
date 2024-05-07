/*
********************************************************************
* rabbitcontrol cpp
*
* written by: Ingo Randolf - 2018
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************
*/

#ifndef DUMMYSERVER_H
#define DUMMYSERVER_H

#include <iostream>
#include <sstream>
#include <vector>

#include "src/servertransporter.h"
#include "src/stream_tools.h"

class DummyServerTransporter : public rcp::ServerTransporter
{
public:
    static void logStream(std::istream& data, const std::string& prefix = "")
    {
        data.seekg (0, data.end);
        size_t length = size_t(data.tellg()); // std::streampos vs. size_t
        data.seekg (0, data.beg);

        std::vector<char> data_vec;
        data_vec.resize(length);

        data.seekg(0);
        data.read(data_vec.data(), length);

        std::cout << prefix << rcp::value_to_string(data_vec) << "\n";
    }

public:
    DummyServerTransporter() : connections(0) {}
    ~DummyServerTransporter() {}

    void addConnection() {
        connections++;
    }
    void clearConnection() {
        connections = 0;
    }

    void testcallall(std::istream& data) {
        for (const auto& kv : receive_cb) {
            (kv.first->*kv.second)(data, *this, nullptr);
        }
    }

    void testcallall(const std::string& data) {

        for (const auto& kv : receive_cb) {
            std::stringstream s;
            s.str(data);
            (kv.first->*kv.second)(s, *this, nullptr);
        }
        std::cout << "--------\n";
    }

public:
    // rcp::ServerTransporter
    virtual void bind(int port) override
    {
        std::cout << "bind" << port << "\n";
    }
    virtual void unbind() override
    {
        std::cout << "ubind\n";
    }

    virtual void sendToOne(std::istream& data, void* /*id*/) override
    {
        logStream(data, "send to one: ");
    }
    virtual void sendToAll(std::istream& data, void* /*excludeId*/) override
    {
        logStream(data, "send to all: ");
    }

    virtual int getConnectionCount() override
    {
        return connections;
    }

private:
    int connections;
};




class ServerUser : public rcp::ServerTransporterReceiver
{
public:
    ServerUser(DummyServerTransporter& s, const std::string& n) : server(s), name(n)
    {
        server.addReceivedCb(this, &ServerTransporterReceiver::received);
    }
    ~ServerUser()
    {
        removeCb();
    }

    void removeCb() {
        server.removeReceivedCb(this);
    }

public:
    // rcp::ServerTransporterReceiver
    void received(std::istream& data, rcp::ServerTransporter& /*transporter*/, void* /*id*/) override
    {
        DummyServerTransporter::logStream(data, "dummy server received:");
    }

private:
    DummyServerTransporter& server;
    std::string name;
};

#endif // DUMMYSERVER_H
