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

        virtual void connect(std::string host, int port) = 0;
        virtual void disconnect() = 0;
        virtual bool isConnected() = 0;

        virtual void send(char* data) = 0;

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
        std::map<ClientTransporterListener*, void(ClientTransporterListener::*)()> connected_cb;
        std::map<ClientTransporterListener*, void(ClientTransporterListener::*)()> disconnected_cb;
        std::map<ClientTransporterListener*, void(ClientTransporterListener::*)(std::istream&)> receive_cb;
    };

}

#endif // CLIENTTRANSPORTER_H
