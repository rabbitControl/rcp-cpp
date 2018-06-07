#ifndef RCPMANAGER_H
#define RCPMANAGER_H

#include <map>
#include <vector>
#include <unordered_set>
#include <climits>

#include "parameter_intern.h"
#include "parameterfactory.h"
#include "iparametermanager.h"

namespace rcp {

class ParameterManager :
        public IParameterManager,
        public std::enable_shared_from_this<ParameterManager>
{
public:    

    static bool isValid(const IParameter& parameter) {
        return parameter.getTypeDefinition().getDatatype() != DATATYPE_MAX_ &&
                parameter.getId() != 0;
    }

    static bool isValid(const ParameterPtr& parameter) {
        return parameter->getTypeDefinition().getDatatype() != DATATYPE_MAX_ &&
                parameter->getId() != 0;
    }

    static bool isGroup(const ParameterPtr& parameter) {
        return isValid(parameter) && parameter->getDatatype() == DATATYPE_GROUP;
    }

    ParameterManager();
    ~ParameterManager();

    // IParameterManager
    virtual ParameterPtr getParameter(const short& id);


    //
    void addParameter(IParameter& parameter, GroupParameterPtr& group);
    void removeParameter(IParameter& parameter);
    void removeParameter(short id);

    //

    BooleanParameter& createBooleanParameter(const std::string& label, GroupParameterPtr& group);
    Int8Parameter& createInt8Parameter(const std::string& label, GroupParameterPtr& group);
    Int16Parameter& createInt16Parameter(const std::string& label, GroupParameterPtr& group);
    Int32Parameter& createInt32Parameter(const std::string& label, GroupParameterPtr& group);
    Int64Parameter& createInt64Parameter(const std::string& label, GroupParameterPtr& group);
    Float32Parameter& createFloat32Parameter(const std::string& label, GroupParameterPtr& group);
    Float64Parameter& createFloat64Parameter(const std::string& label, GroupParameterPtr& group);

    StringParameter& createStringParameter(const std::string& label, GroupParameterPtr& group);

    RGBAParameter& createRGBAParameter(const std::string& label, GroupParameterPtr& group);

    BangParameter& createBangParameter(const std::string& label, GroupParameterPtr& group);

    GroupParameterPtr createGroupParameter(const std::string& label, GroupParameterPtr& group);


    template<typename> friend class Parameter;
    friend class ParameterServer;

private:
    std::shared_ptr<ParameterManager> getShared() {
        return shared_from_this();
    }

    //--------
    short getNextId();
    void _addParameter(ParameterPtr& parameter);
    void _addParameter(ParameterPtr& parameter, GroupParameterPtr& group);
    void _addParameterDirect(const std::string& label, ParameterPtr& parameter, GroupParameterPtr& group);

    virtual void setParameterDirty(IParameter& parameter);
    virtual void setParameterRemoved(ParameterPtr& parameter);

    void removeParameterDirect(ParameterPtr& parameter);

    //--------
    std::unordered_set<short> ids;
    std::map<short, ParameterPtr > params;
    std::map<short, ParameterPtr > dirtyParameter;
    std::map<short, ParameterPtr > removedParameter;
};

}

#endif // RCPMANAGER_H
