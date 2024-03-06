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

#include "parameter_intern.h"

namespace rcp {

void GroupParameter::addChild(ParameterPtr child)
{
    // set parent in child
    // this remove it from the old parameter
    child->setParent(std::dynamic_pointer_cast<GroupParameter>(shared_from_this()));

    // add child to list of children
    if (obj->children.find(child->getId()) == obj->children.end())
    {
        obj->children[child->getId()] = child;
    }
    else
    {
        // child already in list
    }
}

void GroupParameter::addChildInternal(ParameterPtr child)
{
    // set parent in child
    // this remove it from the old parameter
    child->setParentInternal(std::dynamic_pointer_cast<GroupParameter>(shared_from_this()));

    // add child to list of children
    if (obj->children.find(child->getId()) == obj->children.end())
    {
        obj->children[child->getId()] = child;
    }
    else
    {
        // child already in list
    }
}

void GroupParameter::removeChild(ParameterPtr child)
{
    auto it = obj->children.find(child->getId());

    if (it != obj->children.end())
    {
        // remove child
        it->second->clearParentInternal();
        obj->children.erase(it);
    }
}

std::map<int16_t, ParameterPtr >& GroupParameter::children() const
{
    return obj->children;
}

void GroupParameter::dumpChildren(int indent) const
{
    std::string in = "";
    for (int i=0; i<indent; i++)
    {
        in += "  ";
    }

    std::cout << in << getLabel() << " (" << getId() << "):\n";

    in += "  ";

    for (auto& child : obj->children)
    {
        if (child.second->getTypeDefinition().getDatatype() == DATATYPE_GROUP)
        {
            GroupParameter* group = (GroupParameter*)child.second.get();
            group->dumpChildren(indent+1);
        }
        else
        {
            std::cout << in << child.second->getLabel() << " (" << child.second->getId() << ")\n";
            child.second->dump();
        }
    }
}
}

