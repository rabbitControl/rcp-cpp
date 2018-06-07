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
        int length = data.tellg();
        data.seekg (0, data.beg);

        char d[length];
        data.read(d, length);

        std::cout << "send to all: ";
        for (int i=0; i<length; i++) {
            std::cout << (int)d[i] << " ";
        }
        std::cout << "\n";

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
        int length = 1+data.tellg();
        data.seekg (0, data.beg);

        char d[length];
        data.read(d, length);

        std::cout << "myCallback: " << name << " : " << d << "\n";
    }

    void removeCb() {
        server.removeReceivedCb(this);
    }

private:
    DummyServerTransporter& server;
    std::string name;
};

#endif // DUMMYSERVER_H
