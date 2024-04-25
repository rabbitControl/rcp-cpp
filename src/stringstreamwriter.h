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

#ifndef STRINGSTREAMWRITER_H
#define STRINGSTREAMWRITER_H

#include <sstream>

#include "writer.h"

namespace rcp {

/*
* StringStreamWriter writes big-endian to the stream converting if needed
*/
class StringStreamWriter
    : public Writer
{
public:
    StringStreamWriter() {}

    size_t getSize();
    std::stringstream& getBuffer();
    void dump();
    void clear();

public:
    // Writer
    virtual void write(const bool& c) override;
    virtual void write(const char& c) override;
    virtual void write(const uint8_t& c) override;
    virtual void write(const int8_t& c) override;
    virtual void write(const uint16_t& c) override;
    virtual void write(const int16_t& c) override;
    virtual void write(const uint32_t& c) override;
    virtual void write(const int32_t& c) override;
    virtual void write(const uint64_t& c) override;
    virtual void write(const int64_t& c) override;
    virtual void write(const float& c) override;
    virtual void write(const double& c) override;
    virtual void write(const std::string& s, bool prefix = true) override;
    virtual void write(const rcp::Color& s) override;
    virtual void write(const rcp::IPv4& s) override;
    virtual void write(const rcp::IPv6& s) override;
    virtual void write(const char* data, size_t length) override;

private:
    std::stringstream buffer;
};
}


#endif // STRINGSTREAMWRITER_H
