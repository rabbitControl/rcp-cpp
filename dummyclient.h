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

#ifndef DUMMYCLIENT_H
#define DUMMYCLIENT_H

#include <iostream>
#include <sstream>

#include "src/clienttransporter.h"

class DummyClientTransporter : public rcp::ClientTransporter
{
public:
    ~DummyClientTransporter() {}

    virtual void connect(std::string host, int port) {
        std::cout << "connect: " << host << ":" << port << "\n";
        m_isConnected = true;

        for (const auto& kv : connected_cb) {
            (kv.first->*kv.second)();
        }
        std::cout << "--------\n";
    }

    virtual void disconnect() {
        std::cout << "disconnected\n";

        for (const auto& kv : disconnected_cb) {
            (kv.first->*kv.second)();
        }
        std::cout << "--------\n";
    }

    virtual bool isConnected() { return m_isConnected; }

    virtual void send(char* data) {
        std::cout << "send: " << data << "\n";
    }

    //
    void receive(const std::string& data) {
        for (const auto& kv : receive_cb) {
            std::stringstream s;
            s.str(data);
            (kv.first->*kv.second)(s);
        }
        std::cout << "--------\n";
    }

    void testcallall(const std::string& data) {
        connect("host", 123);
        disconnect();
        receive(data);
    }

private:
    bool m_isConnected;

};



class ClientTest : public rcp::ClientTransporterListener
{
public:
    ClientTest(rcp::ClientTransporter& t, const std::string& n) : transporter(t), name(n) {

        transporter.addConnectedCb(this, &ClientTransporterListener::connected);
        transporter.addDisconnectedCb(this, &ClientTransporterListener::disconnected);
        transporter.addReceivedCb(this, &ClientTransporterListener::received);
    }
    ~ClientTest() {
        removeCb();
    }

    void connected() {
        std::cout << "connected cb: " << this << "\n";
    }

    void disconnected() {
        std::cout << "disconnected cb: " << this << "\n";
    }

    void received(std::istream& data) {
        data.seekg (0, data.end);
        int length = 1+data.tellg();
        data.seekg (0, data.beg);

        char d[length];
        data.getline(d, length);

        std::cout << "myCallback: " << name << " : " << d << "\n";
    }

    //
    void removeCb() {
        transporter.removeConnectedCb(this);
        transporter.removeDisconnectedCb(this);
        transporter.removeReceivedCb(this);
    }

private:
    rcp::ClientTransporter& transporter;
    std::string name;
};

#endif // DUMMYCLIENT_H
