/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2023
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCPMANAGER_H
#define RCPMANAGER_H

//#define RCP_MANAGER_NO_LOCKING

#include <map>
#include <vector>
#include <unordered_set>
#include <climits>

#ifndef RCP_MANAGER_NO_LOCKING
#include <mutex>
#endif

#include "parameter_intern.h"
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
        return parameter != nullptr && parameter->getTypeDefinition().getDatatype() != DATATYPE_MAX_ && parameter->getId() != 0;
    }

    static bool isGroup(const ParameterPtr& parameter) {
        return isValid(parameter) && parameter->getDatatype() == DATATYPE_GROUP;
    }

public:
    ParameterManager();
    ~ParameterManager();
	
public:
    //
    void removeParameter(ParameterPtr parameter);
    void removeParameter(short id);

    BooleanParameterPtr createBooleanParameter(const std::string& label, GroupParameterPtr group = nullptr);
    Int8ParameterPtr createInt8Parameter(const std::string& label, GroupParameterPtr group = nullptr);
    Int16ParameterPtr createInt16Parameter(const std::string& label, GroupParameterPtr group = nullptr);
    Int32ParameterPtr createInt32Parameter(const std::string& label, GroupParameterPtr group = nullptr);
    Int64ParameterPtr createInt64Parameter(const std::string& label, GroupParameterPtr group = nullptr);
    Float32ParameterPtr createFloat32Parameter(const std::string& label, GroupParameterPtr group = nullptr);
    Float64ParameterPtr createFloat64Parameter(const std::string& label, GroupParameterPtr group = nullptr);
    StringParameterPtr createStringParameter(const std::string& label, GroupParameterPtr group = nullptr);
    RGBAParameterPtr createRGBAParameter(const std::string& label, GroupParameterPtr group = nullptr);
    BangParameterPtr createBangParameter(const std::string& label, GroupParameterPtr group = nullptr);

    GroupParameterPtr createGroupParameter(const std::string& label, GroupParameterPtr group = nullptr);


    GroupParameterPtr rootGroup() const {
        return m_rootGroup;
    }

    void dumpHierarchy() const {
        m_rootGroup->dumpChildren(0);
        std::flush(std::cout);
    }

    friend class ParameterServer;
    friend class ParameterClient;
	
public:
	// IParameterManager
	ParameterPtr getParameter(int16_t id) override;

private:
	// IParameterManager
	void setParameterDirty(ParameterPtr parameter) override;
	void setParameterRemoved(ParameterPtr parameter) override;
    void addMissingParent(int16_t parentId, ParameterPtr child) override;
	
private:
    short getNextId();
    void _addParameter(ParameterPtr& parameter);
    void _addParameterDirect(const std::string& label, ParameterPtr& parameter, GroupParameterPtr& group);
	void removeParameterDirect(ParameterPtr& parameter);    
    void clear();

    //--------
    std::unordered_set<int16_t> ids;
    std::map<int16_t, ParameterPtr > params;
    std::map<int16_t, ParameterPtr > dirtyParameter;
    std::map<int16_t, ParameterPtr > removedParameter;
    GroupParameterPtr m_rootGroup;

    //
    std::map<int16_t, std::vector<ParameterPtr> > missingParents;
	
private:
	void lock();
	void unlock();
#ifndef RCP_MANAGER_NO_LOCKING
	std::mutex m_mutex;
#endif
};

}

#endif // RCPMANAGER_H
