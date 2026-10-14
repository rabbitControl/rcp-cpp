#include <catch2/catch_test_macros.hpp>

#include <thread>
#include <chrono>
#include <string>

#include <rcp/ParameterClient.h>
#include <rcp/0.1.0/ParameterClient.h>
#include <rcp/0.1.0/types.h>
#include <rcp/0.1.0/InfoData.h>

#include <rcp/1.0/ParameterClient.h>
#include <rcp/1.0/types.h>
#include <rcp/1.0/InfoData.h>
#include <rcp/1.0/ParameterServer.h>

namespace rcp {


class DummyServerTransporter;


class DummyClientTransporter
    : public ClientTransporter
{
    friend class DummyServerTransporter;

    bool m_isConnected{false};

public:
    // ClientTransporter interface
    void connect(std::string host, int port, bool secure)
    {
        std::cout << "connect" << std::endl;
        m_isConnected = true;

        _connected();
    }

    void disconnect()
    {
        if (m_isConnected)
        {
            std::cout << "disconnect" << std::endl;
            m_isConnected = false;
            _disconnected();
        }
    }

    bool isConnected()
    {
        return m_isConnected;
    }

    void send(std::istream & is)
    {
        char first = is.peek();

        // std::cout << "[DummyClientTransporter] send: ";

        // char ch;
        // while(is.get(ch))
        // {
        //     printf("0x%.2x ", (ch & 0xFF));
        // }

        // std::cout << std::endl;



        if (m_serverTransporter)
        {
            sendToServerTransporter(is);
        }
        else
        {
            if (first == v10::PACKET_TYPES_INFO)
            {
                // simulate server sending info packet
                StringStreamWriter writer;
                writer.write(char(v10::PACKET_TYPES_INFO));

                v10::InfoData info(v10::RcpVersion(1, 0), v10::RcpVersion(1, 0));
                info.setApplicationId("Test Dummy Server");
                info.setApplicationVersion("0.0.0");

                info.write(writer, true);

                _received(writer.getBuffer());
            }
            else if (first == v10::PACKET_TYPES_INITIALIZE)
            {
                std::cout << "[DummyClientTransporter] send init\n";
            }
        }
    }

    void send(char *data, size_t size)
    {
        std::cout << "[DummyClientTransporter] send char*: ";

        for (size_t i = 0; i < size; ++i)
        {
            printf("0x%.2x ", (data[i] & 0xFF));
        }

        std::cout << std::endl;


        if (m_serverTransporter)
        {
            auto is = std::istringstream(std::string(data, size));
            sendToServerTransporter(is);
        }
        else
        {
            if (data[0] == v10::PACKET_TYPES_INITIALIZE)
            {
                StringStreamWriter writer;
                writer.write(char(v10::PACKET_TYPES_INITIALIZE));
                writer.write(char(0x81)); // expect one parameter

                _received(writer.getBuffer());
            }
        }
    }

    void sendToServerTransporter(std::istream & is);


    DummyServerTransporter* m_serverTransporter{nullptr};
};

// TEST_CASE("client 0.1.0", "0.1.0")
// {
//     {
//         DummyClientTransporter transpoter;
//         ParameterClient client(transpoter);

//         transpoter.connect("", 10000, false);

//         {
//             // send "server" info
//             auto version = v010::InfoData::create("0.1.0", "Testing");

//             StringStreamWriter writer;
//             writer.write(char(v010::COMMAND_INFO));
//             writer.write(static_cast<char>(v010::PACKET_OPTIONS_DATA));

//             version->write(writer, false);
//             client.received(writer.getBuffer());
//         }

//         {
//             // request info
//             StringStreamWriter writer;
//             writer.write(char(v010::COMMAND_INFO));
//             writer.write(char(0));

//             client.received(writer.getBuffer());
//         }
//     }

//     std::cout << std::endl;
// }


// TEST_CASE("client 1.0", "1.0")
// {
//     {
//         DummyClientTransporter transpoter;
//         ParameterClient client(transpoter);

//         transpoter.connect("", 10000, false);

//         // simulate server sending info packet
//         StringStreamWriter writer;
//         writer.write(char(v10::PACKET_TYPES_INFO));

//         v10::InfoData info(v10::RcpVersion(1, 0).version(), v10::RcpVersion(1, 0).version());
//         info.setApplicationId("Test Dummy Server");
//         info.setApplicationVersion("0.0.0");

//         info.write(writer, true);

//         client.received(writer.getBuffer());
//     }

//     std::cout << std::endl;
// }



class DummyServerTransporter
    : public ServerTransporter
{
    friend class DummyClientTransporter;




public:
    void sendDataToClient(std::istream& is)
    {
        std::vector<char> data;

        std::cout << "--> ";

        char ch;
        while(is.get(ch))
        {
            data.push_back(ch);
            printf("0x%.2x ", (ch & 0xFF));
        }

        std::cout << std::endl;


        std::istringstream* result = new std::istringstream(std::string(data.data(), data.size()));

        m_clientTransporter->_received(*result);

        // std::thread([this, result]
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //     m_clientTransporter->_received(*result);
        //     // delete result;
        // }).detach();
    }


    // ServerTransporter interface
    void bind(int port)
    {
        std::cout << "bind:\n";
    }

    void unbind()
    {
        std::cout << "unbind:\n";
    }

    void sendToOne(std::istream &data, void *id)
    {
        if (m_clientTransporter &&
            m_clientTransporter == id)
        {
            sendDataToClient(data);
        }
    }

    void sendToAll(std::istream &data, void *excludeId)
    {
        if (m_clientTransporter &&
            m_clientTransporter != excludeId)
        {
            sendDataToClient(data);
        }
    }

    int getConnectionCount()
    {
        return m_clientTransporter ? 1 : 0;
    }

    void closeClient(void *id)
    {
        if (m_clientTransporter)
        {
            m_clientTransporter->disconnect();
        }
    }


    DummyClientTransporter* m_clientTransporter{nullptr};
};



TEST_CASE("server client")
{
    DummyServerTransporter serverTransporter;
    auto server = std::make_shared<v10::ParameterServer>(serverTransporter);

    server->setApplicationId("Dummy Test Server");
    server->setApplicationVersion("Server Version 0 0 0");

    auto param = server->createBooleanParameter("Bool");


    DummyClientTransporter clientTransporter;
    // std::shared_ptr<v10::ParameterClient> client = std::make_shared<v10::ParameterClient>(clientTransporter);

    bool v10_triggered = false;

    std::shared_ptr<v010::ParameterClient> client = std::make_shared<v010::ParameterClient>(clientTransporter,
                                                                                            [&v10_triggered]
    {
        // throw std::runtime_error("TODO: switch to v10 client!");
        std::cout << "switch to v10 client!\n";
        v10_triggered = true;
    });


    client->setApplicationId("Dummy Test Client");
    // client->setApplicationVersion("Client Version XXX");


    serverTransporter.m_clientTransporter = &clientTransporter;
    clientTransporter.m_serverTransporter = &serverTransporter;


    clientTransporter.connect("test", 10000, false);

    REQUIRE(v10_triggered);

    // std::thread t([]
    // {
    //     std::this_thread::sleep_for(std::chrono::seconds(100));
    // });
    // t.join();
}

void DummyClientTransporter::sendToServerTransporter(std::istream &is)
{
    if (m_serverTransporter)
    {
        std::vector<char> data;

        std::cout << "<-- ";

        char ch;
        while(is.get(ch))
        {
            data.push_back(ch);
            printf("0x%.2x ", (ch & 0xFF));
        }

        std::cout << std::endl;


        std::istringstream* result = new std::istringstream(std::string(data.data(), data.size()));

        m_serverTransporter->_received(*result, this);

        // std::thread([this, result]
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(777));
        //     m_serverTransporter->_received(*result, this);
        //     // delete result;
        // }).detach();
    }
}



}
