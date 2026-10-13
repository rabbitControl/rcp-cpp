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

#ifndef RCP_V10_PARAMETERWRITER_H
#define RCP_V10_PARAMETERWRITER_H

#include "../Writer.h"

namespace rcp {
namespace v10 {

class IParameter;
using IParameterPtr = std::shared_ptr<IParameter>;

class ParameterWriter
{
public:
    static void write(IParameterPtr parameter, Writer& out, bool all);
    static void writeUpdateValue(IParameterPtr parameter, Writer& out);

    static void writeString(Writer& out, const std::string& str);

private:
    static inline void writeLabel(IParameterPtr parameter, Writer& out);
    static inline void writeDescription(IParameterPtr parameter, Writer& out);
};

}
}

#endif // PARAMETERWRITER_H
