/*
********************************************************************
* RabbitControl - a protocol and data-format for remote control.
*
* https://rabbitcontrol.cc
* https://github.com/rabbitControl/rcp-cpp
*
* This file is part of RabbitControl for c++.
*
* Written by Ingo Randolf, 2018-2026
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at https://mozilla.org/MPL/2.0/.
*********************************************************************
*/

#ifndef RCP_PARAMETERMANAGER_H
#define RCP_PARAMETERMANAGER_H

#include <memory>

#include "IParameter.h"
#include "IGroupParameter.h"


namespace rcp {

class ParameterManager;
using ParameterManagerPtr = std::shared_ptr<ParameterManager>;


class ParameterManager
    : public std::enable_shared_from_this<ParameterManager>
{
    friend rcp::IParameter;

public:
    static ParameterManagerPtr create(const IGroupParameterPtr& root = nullptr) {

        class make_shared_enabler : public ParameterManager
        {
            public: make_shared_enabler(const IGroupParameterPtr& root)
                    : ParameterManager(root)
                {}
        };

        return std::make_shared<make_shared_enabler>(root);
    }


private:
    ParameterManager(const IGroupParameterPtr& root = nullptr)
        : m_rootGroup(root)
    {}

    ~ParameterManager()
    {
        _clear();
    }

public:
    //
    void removeParameter(IParameterPtr parameter)
    {
        removeParameter(parameter->getId());
    }

    void removeParameter(const IdType id)
    {
        if (id == 0)
        {
            return;
        }

#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        auto it = params.find(id);
        if (it == params.end())
        {
            return;
        }

        // add it to removed
        setParameterRemoved(params[id]);

        _removeParameterDirect(params[id]);
    }

//     void dumpHierarchy() const
//     {
// #ifndef RCP_MANAGER_NO_LOCKING
//         // protect lists to be used from multiple threads
//         std::lock_guard<std::recursive_mutex> lock(m_mutex);
// #endif

//         m_rootGroup->dumpChildren(0);
//         std::flush(std::cout);
//     }


public:
    // IParameterManager
    IParameterPtr getParameter(const IdType id) const
    {
#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        auto it = params.find(id);

        if (it != params.end())
        {
            return it->second;
        }

        return nullptr;
    }

    IGroupParameterPtr getRootGroup() const
    {
        return m_rootGroup;
    }




// protected:
    // IParameterManager
    bool setParameterDirty(IParameterPtr parameter)
    {
        if (parameter->getId() == 0)
        {
            // root is never dirty
            return false;
        }

#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        // only add if not already removed
        if (removedParameter.find(parameter->getId()) != removedParameter.end())
        {
            // parameter is removed, don't add
            std::cout << "parameter going to be removed: " << parameter->getId() << "\n";
            return false;
        }

        dirtyParameter[parameter->getId()] = parameter;

        return true;
    }

    bool isParameterDirty(IParameterPtr parameter)
    {
#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        return dirtyParameter.find(parameter->getId()) != dirtyParameter.end();
    }

    void setParameterRemoved(IParameterPtr parameter)
    {
#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        // check if parameter is dirty...
        auto it = dirtyParameter.find(parameter->getId());
        if (it != dirtyParameter.end())
        {
            // remove parameter from dirties
            dirtyParameter.erase(it);
        }

        removedParameter[parameter->getId()] = parameter;
    }

    void addMissingParent(IdType parentId, IParameterPtr child)
    {
#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        // std::cout << "add missing parent: " << parentId << " for parameter: " << child->getId() << std::endl;

        if (missingParents.find(parentId) == missingParents.end())
        {
            // add new list
            missingParents[parentId] = std::vector<IParameterPtr>();
        }

        missingParents[parentId].push_back(child);

        paramsWaitingForParent.push_back(child->getId());
    }


    // TODO: protect?
    /**
     * @brief ParameterManager::_addParameter
     *      called by client
     * @param parameter
     */
    void _addParameter(const IParameterPtr& parameter)
    {
        if (!parameter)
        {
            return;
        }

        // NOTE: this is called from a client
        // make sure parameters are clean by default

#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        // check if already in map
        if (params.find(parameter->getId()) != params.end())
        {
            // parameter is already registered in map... ignore
            return;
        }


        // avoid parameter getting dirty when setting parent
//        parameter->setManager(nullptr);

        // parsed parameters are proxy parameter until they are in params-map
        // proxy parameter are not set as children...
        // so: add parameter as child to its parent
        if (auto parent = parameter->getParent().lock())
        {
            parent->addChildInternal(parameter);
        }
        else if (std::find(paramsWaitingForParent.begin(), paramsWaitingForParent.end(), parameter->getId()) == paramsWaitingForParent.end()
                   && m_rootGroup)
        {
            m_rootGroup->addChildInternal(parameter);
        }


        // add parameter to map
        params[parameter->getId()] = parameter;


        if (parameter->isGroup())
        {
            // resolve missing parents
            // NOTE: this is only relevant for clients
            auto group_parameter = std::dynamic_pointer_cast<IGroupParameter>(parameter);

            auto missing_it = missingParents.find(parameter->getId());
            if (missing_it != missingParents.end())
            {
                for (auto& child : missing_it->second)
                {
                    // avoid child getting dirty when setting parent
                    group_parameter->addChildInternal(child);
                    paramsWaitingForParent.erase(std::find(paramsWaitingForParent.begin(), paramsWaitingForParent.end(), child->getId()));
                    // child->setParentUnchanged();
                }

                missingParents.erase(missing_it);
            }
        }

        // set manager again
//        parameter->setManager(shared_from_this());
    }

    // TODO: protect?
    /**
     * @brief ParameterManager::_addParameterDirect
     *          called by server - parameter expose
     * @param label
     * @param parameter
     * @param group
     */
    void _addParameterDirect(const std::string& label,
                             const IParameterPtr& parameter,
                             const IGroupParameterPtr& group)
    {

#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif

        parameter->setManager(std::enable_shared_from_this<ParameterManager>::shared_from_this());
        // parameter->setManager(this);
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
        else if (m_rootGroup)
        {
            m_rootGroup->addChild(parameter);
        }

        params[parameter->getId()] = parameter;
    }

    void _removeParameterDirect(IParameterPtr& parameter)
    {
#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif


        if (auto p = parameter->getParent().lock())
        {
            p->removeChild(parameter);
        }
        params.erase(parameter->getId());

        // check if this is a group... if so, remove all children without!! adding them to the removed-list
        if (parameter->isGroup())
        {
            auto group = std::dynamic_pointer_cast<IGroupParameter>(parameter);

            for (auto& child : group->children())
            {
                _removeParameterDirect(child.second);
            }
        }
    }

    void _clear()
    {
#ifndef RCP_MANAGER_NO_LOCKING
        // protect lists to be used from multiple threads
        std::lock_guard<std::recursive_mutex> lock(m_mutex);
#endif


        // TODO: remove manager
        for (auto& p : params)
        {
            p.second->setManager(nullptr);
        }


        params.clear();
        dirtyParameter.clear();
        removedParameter.clear();
        missingParents.clear();

        if (m_rootGroup)
        {
            m_rootGroup->clear();
        }
    }



    //--------
    IGroupParameterPtr m_rootGroup;
    std::map<IdType, IParameterPtr > params;
    std::map<IdType, IParameterPtr > dirtyParameter;
    std::map<IdType, IParameterPtr > removedParameter;

    //
    std::map<IdType, std::vector<IParameterPtr> > missingParents;
    std::vector<IdType> paramsWaitingForParent;


    // private:
    void lock()
    {
#ifndef RCP_MANAGER_NO_LOCKING
        m_mutex.lock();
#endif
    }

    void unlock()
    {
#ifndef RCP_MANAGER_NO_LOCKING
        m_mutex.unlock();
#endif
    }

#ifndef RCP_MANAGER_NO_LOCKING
    mutable std::recursive_mutex m_mutex;
#endif
};

}

#endif // PARAMETERMANAGER_H
