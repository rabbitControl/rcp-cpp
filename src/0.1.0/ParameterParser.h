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

#ifndef v010_PARAMETERPARSER_H
#define v010_PARAMETERPARSER_H

#include <iostream>
#include <istream>
#include <string>

#include "types.h"

namespace rcp {

class IParameter;
using IParameterPtr = std::shared_ptr<IParameter>;

class ParameterManager;
using ParameterManagerPtr = std::shared_ptr<ParameterManager>;

namespace v010 {

class ParameterParser
{
public:
    static IParameterPtr parse(std::istream& is, ParameterManagerPtr manager);
    static void parseParameterOptions(IParameterPtr param, std::istream& is, ParameterManagerPtr manager);

    static IParameterPtr parseUpdateValue(std::istream& is);

    // read strings from stream
    static std::string readTinyString(std::istream& is);
    static std::string readShortString(std::istream& is);
    static std::string readLongString(std::istream& is);

    static std::string to_string(datatype_t type);
};

}
}

#endif // v010_PARAMETERPARSER_H
