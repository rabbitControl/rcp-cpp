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

#ifndef RCP_V010_STREAMTOOLS_H
#define RCP_V010_STREAMTOOLS_H

#include <istream>
#include <string>
#include <type_traits>

#include "../StreamTools.h"
#include "../TypeHelper.h"

namespace rcp {
namespace v010 {


template <typename T,
         typename = std::enable_if_t<is_scalar_element_v<T> > >
std::string readStringFromStream(std::istream& is, T /*s*/)
{
    T size;
    is.read(reinterpret_cast<char *>(&size), sizeof(size));

#if BYTE_ORDER == LITTLE_ENDIAN
    size = swap_endian(size);
#endif

    // read to buffer
    std::vector<char> buffer( size );
    is.read(buffer.data(), size);

    return std::string(buffer.data(), size);
}


}
}


#endif // V010STREAMTOOLS_H
