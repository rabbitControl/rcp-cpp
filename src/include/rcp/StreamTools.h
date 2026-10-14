/*
********************************************************************
* rabbitcontrol cpp
*
* written by: Ingo Randolf - 2018
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*********************************************************************
*/

#ifndef RCP_STREAMTOOLS_H
#define RCP_STREAMTOOLS_H

#include <istream>
#include <string>
#include <vector>


#define CHECK_STREAM if (is.eof()) { /* std::cerr << "could not read from stream\n";*/ break;}
#define CHECK_STREAM_RETURN(ret_val) if (is.eof()) { /* std::cerr << "could not read from stream\n"; */return ret_val;}
#define CHECK_STREAM_MSG(msg) if (is.eof()) { /* std::cerr << msg << "\n"; */  break;}
#define CHECK_STREAM_MSG_RETURN(msg) if (is.eof()) { /* std::cerr << msg << "\n"; */ return;}
#define CHECK_STREAM_MSG_RETURN_VAL(msg, ret_val) if (is.eof()) { /* std::cerr << msg << "\n"; */ return ret_val;}


namespace rcp {

template <typename T>
T swap_endian(const T& u)
{
#ifdef CHAR_BIT
    static_assert (CHAR_BIT == 8, "CHAR_BIT != 8");
#endif

    union
    {
        T u;
        unsigned char u8[sizeof(T)];
    } source, dest;

    source.u = u;

    for (size_t k = 0; k < sizeof(T); k++) {
        dest.u8[k] = source.u8[sizeof(T) - k - 1];
    }

    return dest.u;
}

// TODO do this with templates...?
std::string& swap_endian(const std::string &u);



//---------------------------------------------------
// read from stream

template <
    typename T,
    typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr
>
T readFromStream(std::istream& is, const T& /*i*/) {

    T value;
    is.read(reinterpret_cast<char *>(&value), sizeof(T));

#if BYTE_ORDER == LITTLE_ENDIAN
    value = swap_endian(value);
#endif

    return value;
}


bool readFromStream(std::istream& is, const bool& i);
std::string readFromStream(std::istream& is, const std::string& i);



bool type_zero(bool);
std::string type_zero(const std::string&);

// TODO: as template?
std::vector<char> type_zero(const std::vector<char>&);
std::vector<uint32_t> type_zero(const std::vector<uint32_t>&);



//
std::string value_to_string(std::string value);
std::string value_to_string(std::vector<char> value);
std::string value_to_string(bool value);

} // namespace rcp

#endif
