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

#include <iostream>

#include "stringstreamwriter.h"

namespace rcp {

    void StringStreamWriter::write(const bool& c) {
        buffer << static_cast<int8_t>(c);
    }

    void StringStreamWriter::write(const char& c) {
        buffer << static_cast<int8_t>(c);
    }

    void StringStreamWriter::write(const uint8_t& c) {
        buffer << c;
    }
    void StringStreamWriter::write(const int8_t& c) {
        buffer << c;
    }


    void StringStreamWriter::write(const uint16_t& v) {
        write(static_cast<int16_t>(v));
    }
    void StringStreamWriter::write(const int16_t& v) {
#if BYTE_ORDER == LITTLE_ENDIAN
        buffer << static_cast<unsigned char>(v >> 8);
        buffer << static_cast<unsigned char>(v);
#else
        buffer << static_cast<unsigned char>(v);
        buffer << static_cast<unsigned char>(v >> 8);
#endif
    }


    void StringStreamWriter::write(const uint32_t& v) {
        write(static_cast<int32_t>(v));
    }
    void StringStreamWriter::write(const int32_t& v) {
#if BYTE_ORDER == LITTLE_ENDIAN
        buffer << static_cast<unsigned char>(v >> 24);
        buffer << static_cast<unsigned char>(v >> 16);
        buffer << static_cast<unsigned char>(v >> 8);
        buffer << static_cast<unsigned char>(v);
#else
        buffer << static_cast<unsigned char>(v);
        buffer << static_cast<unsigned char>(v >> 8);
        buffer << static_cast<unsigned char>(v >> 16);
        buffer << static_cast<unsigned char>(v >> 24);
#endif

    }


    void StringStreamWriter::write(const uint64_t& v) {
        write(static_cast<int64_t>(v));
    }
    void StringStreamWriter::write(const int64_t& v) {
#if BYTE_ORDER == LITTLE_ENDIAN
        buffer << static_cast<unsigned char>(v >> 56);
        buffer << static_cast<unsigned char>(v >> 48);
        buffer << static_cast<unsigned char>(v >> 40);
        buffer << static_cast<unsigned char>(v >> 32);

        buffer << static_cast<unsigned char>(v >> 24);
        buffer << static_cast<unsigned char>(v >> 16);
        buffer << static_cast<unsigned char>(v >> 8);
        buffer << static_cast<unsigned char>(v);
#else
        buffer << static_cast<unsigned char>(v);
        buffer << static_cast<unsigned char>(v >> 8);
        buffer << static_cast<unsigned char>(v >> 16);
        buffer << static_cast<unsigned char>(v >> 24);

        buffer << static_cast<unsigned char>(v >> 32);
        buffer << static_cast<unsigned char>(v >> 40);
        buffer << static_cast<unsigned char>(v >> 48);
        buffer << static_cast<unsigned char>(v >> 56);
#endif
    }


    void StringStreamWriter::write(const float& value) {
        float fv = value;
        write(reinterpret_cast<uint32_t&>(fv));
    }


    void StringStreamWriter::write(const double& value) {
        double dv = value;
        write(reinterpret_cast<uint64_t&>(dv));
    }


    void StringStreamWriter::write(const std::string& s, bool prefix) {
        if (prefix) {
            write(static_cast<uint32_t>(s.length()));
        }
        buffer << s;
    }

    void StringStreamWriter::write(const rcp::Color& s) {
        write(s.getValue());
    }

    void StringStreamWriter::write(const rcp::IPv4& s) {
        write(s.getAddress());
    }

    void StringStreamWriter::write(const rcp::IPv6& s) {
        for (int i=0; i<4; i++) {
            write(static_cast<uint32_t>(s.getAddress(i)));
        }
    }

    void StringStreamWriter::write(const char* data, uint32_t length) {
        buffer.write(data, (std::streamsize)length);
    }
}

