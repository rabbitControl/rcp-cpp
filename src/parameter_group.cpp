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

#include "parameter_intern.h"

namespace rcp {

    void GroupParameter::addChild(IParameter& child) {

        ParameterPtr p = child.newReference();
        addChild(p);
    }

    void GroupParameter::addChild(ParameterPtr& child) {

        std::map<short, std::shared_ptr<IParameter > >::iterator it = obj->children.find(child->getId());
        if (it == obj->children.end()) {
            obj->children[child->getId()] = child;
        }

        // set parent in child
        child->setParent(*this);
    }

    void GroupParameter::removeChild(IParameter& child) {

        auto it = obj->children.find(child.getId());
        if (it != obj->children.end()) {
            // found! - remove
            child.clearParent();
            obj->children.erase(child.getId());
        }
    }

    void GroupParameter::removeChild(ParameterPtr& child) {
        removeChild(*child.get());
    }

    void GroupParameter::dumpChildren(int indent) {
        std::string in = "";
        for (int i=0; i<indent; i++) {
            in += "  ";
        }

        std::cout << in << getLabel() << " (" << getId() << "):\n";

        in += "  ";

        for (auto& child : obj->children) {

            if (child.second->getTypeDefinition().getDatatype() == DATATYPE_GROUP) {
                GroupParameter* group = (GroupParameter*)child.second.get();
                group->dumpChildren(indent+1);
            } else {
                std::cout << in << child.second->getLabel() << " (" << child.second->getId() << ")\n";
            }
        }
    }
}

