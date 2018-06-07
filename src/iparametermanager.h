#ifndef IPARAMETERMANAGER_H
#define IPARAMETERMANAGER_H

#include <memory>

#include "iparameter.h"

namespace rcp {

    class IParameterManager {
    public:
        virtual ParameterPtr getParameter(const short& id) = 0;
        virtual void setParameterDirty(IParameter& parameter) = 0;
        virtual void setParameterRemoved(ParameterPtr& parameter) = 0;
    };

    typedef std::shared_ptr<IParameterManager> ParameterManagerPtr;
}


#endif // IPARAMETERMANAGER_H
