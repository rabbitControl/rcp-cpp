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

#ifndef RCP_V10_PARAMETERPARSER_H
#define RCP_V10_PARAMETERPARSER_H

#include <istream>
#include <string>

namespace rcp {

class ParameterManager;
using ParameterManagerPtr = std::shared_ptr<ParameterManager>;

namespace v10 {

class IParameter;
using IParameterPtr = std::shared_ptr<IParameter>;

class ParameterParser
{
public:
    static std::string readString(std::istream& is);

    static IParameterPtr parse(std::istream& is, ParameterManagerPtr manager);
    static void parseParameterOptions(IParameterPtr param, std::istream& is, ParameterManagerPtr manager);

    static IParameterPtr parseUpdateValue(std::istream& is);
};


}
}

#endif // RCP_V10_PARAMETERPARSER_H
