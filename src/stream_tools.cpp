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

#include "stream_tools.h"

namespace rcp {

    //---------------------------------------------------
    // swap string
    std::string& swap_endian(std::string &u) {
        return u;
    }

    //---------------------------------------------------
    // read from stream
    bool readFromStream(std::istream& is, bool const& i) {
        bool value;
        is.read(reinterpret_cast<char*>(&value), sizeof(bool));
        return value;
    }

    std::string readFromStream(std::istream& is, std::string const& i)
    {
        return i;
    }

    //---------------------------------------------------
    // read strings from stream
    std::string readTinyString(std::istream& is) {

        char size = 0;
        return readStringFromStream(is, size);
    }

    std::string readShortString(std::istream& is) {

        uint16_t size = 0;
        return readStringFromStream(is, size);
    }

    std::string readLongString(std::istream& is) {

        uint32_t size = 0;
        return readStringFromStream(is, size);
    }


    // to string
    std::string value_to_string(std::string value)
    {
        return value;
    }

    std::string value_to_string(TinyString value)
    {
        return value;
    }
}
