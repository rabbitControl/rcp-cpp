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

#include "streamwriter.h"

namespace rcp {

    void StreamWriter::write(const bool& c) {
        os << c;
    }

    void StreamWriter::write(const char& c) {
        os << c;
    }

    void StreamWriter::write(const uint8_t& c) {
        os << c;
    }
    void StreamWriter::write(const int8_t& c) {
        os << c;
    }


    void StreamWriter::write(const uint16_t& c) {
        os << c;
    }
    void StreamWriter::write(const int16_t& c) {
        os << c;
    }


    void StreamWriter::write(const uint32_t& c) {
        os << c;
    }
    void StreamWriter::write(const int32_t& c) {
        os << c;
    }


    void StreamWriter::write(const uint64_t& c) {
        os << c;
    }
    void StreamWriter::write(const int64_t& c) {
        os << c;
    }


    void StreamWriter::write(const float& c) {
        os << c;
    }
    void StreamWriter::write(const double& c) {
        os << c;
    }


    void StreamWriter::write(const std::string& s, bool prefix) {
        if (prefix) {
            write(static_cast<uint32_t>(s.length()));
        }
        os << s;
    }


    void StreamWriter::write(const rcp::Color& s) {
        os << s.getValue();
    }


    void StreamWriter::write(const rcp::IPv4& s) {
        os << s.getAddress();
    }

    void StreamWriter::write(const rcp::IPv6& s) {
        for (int i=0; i<4; i++) {
            os << static_cast<uint32_t>(s.getAddress(i));
        }
    }


    void StreamWriter::write(const char* data, size_t length) {
        os.write(data, length);
    }

} // namespace
