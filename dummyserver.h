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

#include "src/servertransporter.h"

class DummyServerTransporter : public rcp::ServerTransporter
{
public:
    DummyServerTransporter() : connections(0) {}
    ~DummyServerTransporter() {}

    virtual void bind(int port) {
        std::cout << "bind" << port << "\n";
    }
    virtual void unbind() {
        std::cout << "ubind\n";
    }

    virtual void sendToOne(std::istream& data, void*id) {
        std::cout << "send to one\n";
    }
    virtual void sendToAll(std::istream& data, void* excludeId)  {

        data.seekg (0, data.end);
        size_t length = size_t(data.tellg()); // std::streampos vs. size_t
        data.seekg (0, data.beg);

        char *d = new char[length];
        if (d)
        {
            data.read(d, length);

            std::cout << "send to all: ";
            for (size_t i=0; i<length; i++) {
                std::cout << int(d[i]) << " ";
            }
            std::cout << "\n";

            delete[] d;
        }
    }

    virtual int getConnectionCount() {
        return connections;
    }

    void addConnection() {
        connections++;
    }
    void clearConnection() {
        connections = 0;
    }

    void testcallall(const std::string& data) {

        for (const auto& kv : receive_cb) {
            std::stringstream s;
            s.str(data);
            (kv.first->*kv.second)(s, *this, nullptr);
        }
        std::cout << "--------\n";
    }

private:
    int connections;
};




class ServerUser : public rcp::ServerTransporterReceiver
{
public:
    ServerUser(DummyServerTransporter& s, const std::string& n) : server(s), name(n) {
        server.addReceivedCb(this, &ServerTransporterReceiver::received);
    }
    ~ServerUser() {
        removeCb();
    }

    void received(std::istream& data, rcp::ServerTransporter& transporter, void* id) {

        data.seekg (0, data.end);
        size_t length = size_t(data.tellg()); // std::streampos vs size_t
        data.seekg (0, data.beg);

        char *d = new char[length + 1];
        if (d)
        {
            data.read(d, length);
            d[length] = 0;

            std::cout << "myCallback: " << name << " : " << d << "\n";

            delete[] d;
        }
    }

    void removeCb() {
        server.removeReceivedCb(this);
    }

private:
    DummyServerTransporter& server;
    std::string name;
};

#endif // DUMMYSERVER_H
