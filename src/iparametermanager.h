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

#ifndef IPARAMETERMANAGER_H
#define IPARAMETERMANAGER_H

#include <memory>

#include "iparameter.h"

namespace rcp {

    class IParameterManager {

        template<typename> friend class Parameter;
        friend class ParameterServer;
        friend class ParameterClient;

    public:
        virtual ParameterPtr getParameter(int16_t id) const = 0;
        virtual ParameterPtr getRootGroup() const = 0;

    private:
        virtual void setParameterDirty(ParameterPtr parameter) = 0;
        virtual bool isParameterDirty(ParameterPtr parameter) = 0;
        virtual void setParameterRemoved(ParameterPtr parameter) = 0;
        virtual void addMissingParent(int16_t parentId, ParameterPtr child) = 0;
    };

    typedef std::shared_ptr<IParameterManager> ParameterManagerPtr;
}


#endif // IPARAMETERMANAGER_H
