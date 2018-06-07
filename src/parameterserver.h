#ifndef RCPSERVER_H
#define RCPSERVER_H

#include <set>

#include "servertransporter.h"
#include "parametermanager.h"

namespace rcp {

    class Packet;
class ParameterServer : public ServerTransporterReceiver
{
public:
    ParameterServer();
    ParameterServer(ServerTransporter& transporter);
    ~ParameterServer();

    void dispose();

    bool addTransporter(ServerTransporter& transporter);
    bool removeTransporter(ServerTransporter& transporter);
    int getConnectionCount();

    bool update();


    // ServerTransporterReceiver
    void received(std::istream& data, ServerTransporter& transporter, void* id);


    GroupParameterPtr& getRoot() { return root; }

    ParameterPtr getParameter(const short& id) {
        return parameterManager->getParameter(id);
    }

    //
    void addParameter(IParameter& parameter) {
        parameterManager->addParameter(parameter, root);
    }
    void addParameter(IParameter& parameter, GroupParameterPtr& group) {
        parameterManager->addParameter(parameter, group);
    }
    void removeParameter(IParameter& parameter) {
        parameterManager->removeParameter(parameter);
    }
    void removeParameter(short id) {
        parameterManager->removeParameter(id);
    }

    // parameter creater
    BooleanParameter& createBooleanParameter(const std::string& label) {
        return parameterManager->createBooleanParameter(label, root);
    }
    BooleanParameter& createBooleanParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createBooleanParameter(label, group);
    }

    Int8Parameter& createInt8Parameter(const std::string& label) {
        return parameterManager->createInt8Parameter(label, root);
    }
    Int8Parameter& createInt8Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt8Parameter(label, group);
    }

    Int16Parameter& createInt16Parameter(const std::string& label) {
        return parameterManager->createInt16Parameter(label, root);
    }
    Int16Parameter& createInt16Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt16Parameter(label, group);
    }

    Int32Parameter& createInt32Parameter(const std::string& label) {
        return parameterManager->createInt32Parameter(label, root);
    }
    Int32Parameter& createInt32Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt32Parameter(label, group);
    }

    Int64Parameter& createInt64Parameter(const std::string& label) {
        return parameterManager->createInt64Parameter(label, root);
    }
    Int64Parameter& createInt64Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createInt64Parameter(label, group);
    }

    Float32Parameter& createFloat32Parameter(const std::string& label) {
        return parameterManager->createFloat32Parameter(label, root);
    }
    Float32Parameter& createFloat32Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createFloat32Parameter(label, group);
    }

    Float64Parameter& createFloat64Parameter(const std::string& label) {
        return parameterManager->createFloat64Parameter(label, root);
    }
    Float64Parameter& createFloat64Parameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createFloat64Parameter(label, group);
    }


    StringParameter& createStringParameter(const std::string& label) {
        return parameterManager->createStringParameter(label, root);
    }
    StringParameter& createStringParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createStringParameter(label, group);
    }


    RGBAParameter& createRGBAParameter(const std::string& label) {
        return parameterManager->createRGBAParameter(label, root);
    }
    RGBAParameter& createRGBAParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createRGBAParameter(label, group);
    }



    BangParameter& createBangParameter(const std::string& label) {
        return parameterManager->createBangParameter(label, root);
    }
    BangParameter& createBangParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createBangParameter(label, group);
    }

    GroupParameterPtr createGroupParameter(const std::string& label) {
        return parameterManager->createGroupParameter(label, root);
    }
    GroupParameterPtr createGroupParameter(const std::string& label, GroupParameterPtr& group) {
        return parameterManager->createGroupParameter(label, group);
    }


    void dumpHierarchy() {
        root->dumpChildren(0);
    }

private:
    void _init(ServerTransporter& transporter, void *id);
    bool _update(Packet& Packet, ServerTransporter& transporter, void *id);
    void _version(ServerTransporter& transporter, void *id);
    void _sendParameterFull(ParameterPtr& parameter, ServerTransporter& transporter, void *id);

    std::shared_ptr<GroupParameter> root;
    std::shared_ptr<ParameterManager> parameterManager;
    std::vector<std::reference_wrapper<ServerTransporter> > transporterList;

//    Events:
//    onError(Exception ex);
};

}
#endif // RCPSERVER_H
