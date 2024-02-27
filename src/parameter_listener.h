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

#include <string>

namespace rcp
{

class ParameterListener
{
public:
    virtual void onLabelChanged(const std::string& /*label*/, const std::string& /*oldLabel*/) {}
    virtual void onLanguageLabelChanged(const std::string& /*languageCode*/, const std::string& /*label*/, const std::string& /*oldLabel*/) {}
    virtual void onDescriptionChanged(const std::string& /*description*/, const std::string& /*oldDescription*/) {}
    virtual void onLanguageDescriptionChanged(const std::string& /*languageCode*/, const std::string& /*description*/, const std::string& /*oldDescription*/) {}
    virtual void onTagsChanged(const std::string& /*tags*/, const std::string& /*oldTags*/) {}
    virtual void onOrderChanged(const int32_t /*order*/, const int32_t /*oldOrder*/) {}
    virtual void onParentChanged(const int16_t /*parentId*/, const int16_t /*oldParentId*/) {}
//    /*TODO*/virtual void onWidgetChange() {}
    virtual void onUserdataChanged(const std::vector<char>& /*data*/, const std::vector<char>& /*oldData*/) {}
    virtual void onUseridChanged(const std::string& /*userid*/, const std::string& /*oldUserid*/) {}
    virtual void onReadonlyChanged(bool /*readonly*/) {}
};

}

#endif // PARAMETER_LISTENER_H
