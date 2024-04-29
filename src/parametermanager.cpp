/*
********************************************************************
* rabbitcontrol - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of rabbitcontrol for c++.
*
* Written by Ingo Randolf, 2018-2024
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#include "parametermanager.h"

namespace rcp {

ParameterManager::ParameterManager()
    : m_rootGroup(std::make_shared<GroupParameter>(0))
{
    m_rootGroup->setLabel("root");
}

ParameterManager::~ParameterManager() {
}

void ParameterManager::removeParameter(ParameterPtr parameter) {
    removeParameter(parameter->getId());
}

void ParameterManager::removeParameter(int16_t id)
{
    if (id == 0)
    {
        return;
    }

    auto it = params.find(id);
    if (it == params.end())
    {
        return;
    }

    // add it to removed
    setParameterRemoved(params[id]);

    removeParameterDirect(params[id]);
}

void ParameterManager::removeParameterDirect(ParameterPtr& parameter) {

    if (!isValid(*parameter)) return;

    // erase from available ids
    auto id_it = std::find(ids.begin(), ids.end(), parameter->getId());
    if (id_it != ids.end()) {
        ids.erase(id_it);
    } else {
        std::cerr << "ParameterManager::removeParameterDirect - could not find id in id list\n";
    }

    if (auto p = parameter->getParent().lock()) {
        p->removeChild(parameter);
    }
    params.erase(parameter->getId());

    // check if this is a group... if so, remove all children without!! adding them to the removed-list
    if (parameter->getTypeDefinition().getDatatype() == DATATYPE_GROUP) {
        GroupParameterPtr gp = std::dynamic_pointer_cast<GroupParameter>(parameter);
        for (auto& child : gp->children()) {
            removeParameterDirect(child.second);
        }
    }
}


//--------------------------------------------
//--------------------------------------------
// parameter creation
//--------------------------------------------
//--------------------------------------------
BooleanParameterPtr ParameterManager::createBooleanParameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        BooleanParameterPtr p = std::make_shared<BooleanParameter>(id, false);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

Int8ParameterPtr ParameterManager::createInt8Parameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        Int8ParameterPtr p = std::make_shared<Int8Parameter>(id, 0);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

Int16ParameterPtr ParameterManager::createInt16Parameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        Int16ParameterPtr p = std::make_shared<Int16Parameter>(id, 0);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

Int32ParameterPtr ParameterManager::createInt32Parameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        Int32ParameterPtr p = std::make_shared<Int32Parameter>(id, 0);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

Int64ParameterPtr ParameterManager::createInt64Parameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        Int64ParameterPtr p = std::make_shared<Int64Parameter>(id, 0);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

Float32ParameterPtr ParameterManager::createFloat32Parameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        Float32ParameterPtr p = std::make_shared<Float32Parameter>(id, 0.0f);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

Float64ParameterPtr ParameterManager::createFloat64Parameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        Float64ParameterPtr p = std::make_shared<Float64Parameter>(id, 0.0);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}


StringParameterPtr ParameterManager::createStringParameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        StringParameterPtr p = std::make_shared<StringParameter>(id);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

RGBAParameterPtr ParameterManager::createRGBAParameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        RGBAParameterPtr p = std::make_shared<RGBAParameter>(id);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

BangParameterPtr ParameterManager::createBangParameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        BangParameterPtr p = std::make_shared<BangParameter>(id);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}

GroupParameterPtr ParameterManager::createGroupParameter(const std::string& label, GroupParameterPtr group)
{
    int16_t id = getNextId();
    if (id != 0)
    {
        GroupParameterPtr p = std::make_shared<GroupParameter>(id);
        _addParameterDirect(label, (ParameterPtr&)p, group);

        return p;
    }

    // ?? - yeah? or use some options here?
    throw std::runtime_error("no valid id...");
}



ParameterPtr ParameterManager::getParameter(const int16_t id) const
{
    auto it = params.find(id);

    if (it != params.end())
    {
        return it->second;
    }

    return nullptr;
}


// private functions


int16_t ParameterManager::getNextId()
{
    for (unsigned short i=1; i<USHRT_MAX; i++)
    {
        if (ids.find(static_cast<int16_t>(i)) == ids.end())
        {
            int16_t i_s = static_cast<int16_t>(i);
            ids.insert(i_s);
            return i_s;
        }
    }

    // return invalid id 0
    return 0;
}


/**
     * @brief ParameterManager::_addParameter
     *      called by client
     * @param parameter
     */
void ParameterManager::_addParameter(ParameterPtr& parameter)
{
    // NOTE: this is called from a client
    // make sure parameters are clean by default

    // check if already in map
    if (params.find(parameter->getId()) != params.end())
    {
        // parameter is already registered in map... ignore
        return;
    }

    // need to reserve id
    if (std::find(ids.begin(), ids.end(), parameter->getId()) != ids.end())
    {
        // huh - parameter is not in parameter cache, but id already taken!?
        std::cerr << "inconsistency in id,parameter list\n";
    }

    // in any case: reserve that id
    ids.insert(parameter->getId());

    // avoid parameter getting dirty when setting parent
    //        parameter->setManager(nullptr);

    // parsed parameters are proxy parameter until they are in params-map
    // proxy parameter are not set as children...
    // so: add parameter as child to its parent
    if (auto parent = parameter->getParent().lock())
    {
        parent->addChildInternal(parameter);
    }
    else if (!parameter->waitForParent())
    {
        m_rootGroup->addChildInternal(parameter);
    }

    // add parameter to map
    params[parameter->getId()] = parameter;

    if (isGroup(parameter))
    {
        // resolve missing parents
        // NOTE: this is only relevant for clients
        GroupParameterPtr group_parameter = std::dynamic_pointer_cast<GroupParameter>(parameter);

        auto missing_it = missingParents.find(parameter->getId());
        if (missing_it != missingParents.end())
        {
            for (ParameterPtr& child : missing_it->second)
            {
                // avoid child getting dirty when setting parent
                group_parameter->addChildInternal(child);
                child->setParentUnchanged();
            }

            missingParents.erase(missing_it);
        }
    }

    // set manager again
    //        parameter->setManager(shared_from_this());
}

void ParameterManager::addMissingParent(int16_t parentId, ParameterPtr child)
{
    if (missingParents.find(parentId) == missingParents.end())
    {
        // add new list
        missingParents[parentId] = std::vector<ParameterPtr>();
    }

    missingParents[parentId].push_back(child);
}

/**
     * @brief ParameterManager::_addParameterDirect
     *          called by server - parameter expose
     * @param label
     * @param parameter
     * @param group
     */
void ParameterManager::_addParameterDirect(const std::string& label, ParameterPtr& parameter, GroupParameterPtr& group) {

    parameter->setManager(shared_from_this());
    parameter->setLabel(label);

    // called on server - parameters are dirty by default
    // (so we send it out...)
    // TODO: is this needed? - INITIALIZE would force writing a parameter anyways...
    parameter->setDirty();

    // add to group
    // mark as dirty?
    if (group != nullptr)
    {
        group->addChild(parameter);
    }
    else
    {
        m_rootGroup->addChild(parameter);
    }

    params[parameter->getId()] = parameter;
}


bool ParameterManager::setParameterDirty(ParameterPtr parameter)
{
    if (parameter->getId() == 0)
    {
        // root is never dirty
        return false;
    }

#ifndef RCP_MANAGER_NO_LOCKING
    // protect lists to be used from multiple threads
    std::lock_guard<std::mutex> lock(m_mutex);
#endif

    // only add if not already removed
    if (removedParameter.find(parameter->getId()) != removedParameter.end()) {
        // parameter is removed, don't add
        std::cout << "parameter going to be removed: " << parameter->getId() << "\n";
        return false;
    }

    dirtyParameter[parameter->getId()] = parameter;

    return true;
}

bool ParameterManager::isParameterDirty(ParameterPtr parameter)
{
#ifndef RCP_MANAGER_NO_LOCKING
    // protect lists to be used from multiple threads
    std::lock_guard<std::mutex> lock(m_mutex);
#endif

    return dirtyParameter.find(parameter->getId()) != dirtyParameter.end();
}

void ParameterManager::setParameterRemoved(ParameterPtr parameter)
{
#ifndef RCP_MANAGER_NO_LOCKING
    // protect lists to be used from multiple threads
    std::lock_guard<std::mutex> lock(m_mutex);
#endif

    // check if parameter is dirty...
    auto it = dirtyParameter.find(parameter->getId());
    if (it != dirtyParameter.end()) {
        // remove parameter from dirties
        dirtyParameter.erase(it);
    }

    removedParameter[parameter->getId()] = parameter;
}

void ParameterManager::clear()
{
#ifndef RCP_MANAGER_NO_LOCKING
    // protect lists to be used from multiple threads
    std::lock_guard<std::mutex> lock(m_mutex);
#endif

    ids.clear();
    params.clear();
    dirtyParameter.clear();
    removedParameter.clear();
    m_rootGroup->children().clear();
    missingParents.clear();
}

void ParameterManager::lock()
{
#ifndef RCP_MANAGER_NO_LOCKING
    m_mutex.lock();
#endif
}

void ParameterManager::unlock()
{
#ifndef RCP_MANAGER_NO_LOCKING
    m_mutex.unlock();
#endif
}


GroupParameterPtr ParameterManager::rootGroup() const
{
    return m_rootGroup;
}

ParameterPtr ParameterManager::getRootGroup() const
{
    return m_rootGroup;
}

void ParameterManager::dumpHierarchy() const
{
    m_rootGroup->dumpChildren(0);
    std::flush(std::cout);
}

} // namespace rcp
