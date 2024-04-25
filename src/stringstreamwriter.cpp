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

#include <iostream>

#include "stringstreamwriter.h"

namespace rcp {


size_t StringStreamWriter::getSize()
{
    return buffer.str().size();
}

std::stringstream& StringStreamWriter::getBuffer()
{
    buffer.seekg(0);
    return buffer;
}

void StringStreamWriter::dump()
{
    buffer.seekg(0, buffer.end);
    size_t len = size_t(buffer.tellg());
    buffer.seekg(0, buffer.beg);

    std::vector<char> data;
    data.resize(len);

    buffer.seekg(0);
    buffer.read(data.data(), len);

    std::cout << value_to_string(data) << "\n";
}

void StringStreamWriter::clear()
{
    buffer.str("");
}


void StringStreamWriter::write(const bool& c)
{
    buffer.write((const char*)&c, 1);
}

void StringStreamWriter::write(const char& c)
{
    buffer.write(&c, 1);
}

void StringStreamWriter::write(const uint8_t& c)
{
    buffer.write((const char*)&c, 1);
}
void StringStreamWriter::write(const int8_t& c)
{
    buffer.write((const char*)&c, 1);
}


void StringStreamWriter::write(const uint16_t& v)
{
    write(static_cast<int16_t>(v));
}

void StringStreamWriter::write(const int16_t& v)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    buffer.write(((const char*)&v) + 1, 1);
    buffer.write(((const char*)&v), 1);
#else
    buffer.write(((const char*)&v), sizeof(int16_t));
#endif
}


void StringStreamWriter::write(const uint32_t& v)
{
    write(static_cast<int32_t>(v));
}

void StringStreamWriter::write(const int32_t& v)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    buffer.write(((const char*)&v) + 3, 1);
    buffer.write(((const char*)&v) + 2, 1);
    buffer.write(((const char*)&v) + 1, 1);
    buffer.write(((const char*)&v), 1);
#else
    buffer.write(((const char*)&v), sizeof(int32_t));
#endif
}


void StringStreamWriter::write(const uint64_t& v)
{
    write(static_cast<int64_t>(v));
}

void StringStreamWriter::write(const int64_t& v)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    buffer.write(((const char*)&v) + 7, 1);
    buffer.write(((const char*)&v) + 6, 1);
    buffer.write(((const char*)&v) + 5, 1);
    buffer.write(((const char*)&v) + 4, 1);

    buffer.write(((const char*)&v) + 3, 1);
    buffer.write(((const char*)&v) + 2, 1);
    buffer.write(((const char*)&v) + 1, 1);
    buffer.write(((const char*)&v), 1);
#else
    buffer.write(((const char*)&v), sizeof(int64_t));
#endif
}


void StringStreamWriter::write(const float& value)
{
    float fv = value;
    write(reinterpret_cast<uint32_t&>(fv));
}


void StringStreamWriter::write(const double& value)
{
    double dv = value;
    write(reinterpret_cast<uint64_t&>(dv));
}


void StringStreamWriter::write(const std::string& s, bool prefix)
{
    if (prefix)
    {
        write(static_cast<uint32_t>(s.length()));
    }
    buffer << s;
}

void StringStreamWriter::write(const rcp::Color& s)
{
    write(s.getValue());
}

void StringStreamWriter::write(const rcp::IPv4& s)
{
    write(s.getAddress());
}

void StringStreamWriter::write(const rcp::IPv6& s)
{
    for (int i=0; i<4; i++)
    {
        write(static_cast<uint32_t>(s.getAddress(i)));
    }
}

void StringStreamWriter::write(const char* data, size_t length)
{
    buffer.write(data, length);
}

}

