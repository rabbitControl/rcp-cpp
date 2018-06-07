#ifndef SERVERTRANSPORTER_H
#define SERVERTRANSPORTER_H

#include <map>

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
        std::map<ServerTransporterReceiver*, void(ServerTransporterReceiver::*)(std::istream&, ServerTransporter&, void*)> receive_cb;
    };
}


#endif // SERVERTRANSPORTER_H
