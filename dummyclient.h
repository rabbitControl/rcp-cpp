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
