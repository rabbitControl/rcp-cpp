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

#ifndef PARAMETER_LISTENER_H
#define PARAMETER_LISTENER_H

#include "iparameter.h"

#include <string>

namespace rcp
{

class ParameterListener
{
public:
    virtual void onLabelChanged(IParameter* /*parameter*/, const std::string& /*oldLabel*/) {}
    virtual void onLanguageLabelChanged(IParameter* /*parameter*/, const std::string& /*languageCode*/, const std::string& /*oldLabel*/) {}
    virtual void onDescriptionChanged(IParameter* /*parameter*/, const std::string& /*oldDescription*/) {}
    virtual void onLanguageDescriptionChanged(IParameter* /*parameter*/, const std::string& /*languageCode*/, const std::string& /*oldDescription*/) {}
    virtual void onTagsChanged(IParameter* /*parameter*/, const std::string& /*oldTags*/) {}
    virtual void onOrderChanged(IParameter* /*parameter*/, const int32_t /*oldOrder*/) {}
    virtual void onParentChanged(IParameter* /*parameter*/, const int16_t /*oldParentId*/) {}
//    /*TODO*/virtual void onWidgetChange() {}
    virtual void onUserdataChanged(IParameter* /*parameter*/, const std::vector<char>& /*oldData*/) {}
    virtual void onUseridChanged(IParameter* /*parameter*/, const std::string& /*oldUserid*/) {}
    virtual void onReadonlyChanged(IParameter* /*parameter*/) {}

    // parameter became dirty
    virtual void onDirty(IParameter* /*parameter*/) {}
};

}

#endif // PARAMETER_LISTENER_H
