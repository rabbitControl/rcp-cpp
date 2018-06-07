#include "parameter_intern.h"
//#include "parameterfactory.h"

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

        std::cout << in << getLabel() << ":\n";

        in += "  ";

        for (auto& child : obj->children) {

            if (child.second->getTypeDefinition().getDatatype() == DATATYPE_GROUP) {
                GroupParameter* group = (GroupParameter*)child.second.get();
                group->dumpChildren(indent+1);
            } else {
                std::cout << in << child.second->getLabel() << "\n";
            }
        }
    }
}

