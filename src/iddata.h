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

#ifndef IDDATA_H
#define IDDATA_H

#include "writeable.h"
#include "stream_tools.h"

namespace rcp {

class IdData;
typedef std::shared_ptr<IdData> IdDataPtr;

class IdData
    : public Writeable
{
public:
    //----------------------------------------
    // parser
    static IdDataPtr parse(std::istream& is) {

        // read mandatory
        const int16_t parameter_id = readFromStream(is, parameter_id);
        return std::make_shared<IdData>(parameter_id);
    }

    static inline IdDataPtr create(const int16_t id) {
        return std::make_shared<IdData>(id);
    }

    IdData(const int16_t id) :
        m_id(id)
    {}

    int16_t getId() { return m_id; }


    //----------------------------------------
    // interface Writeable
    virtual void write(Writer& out, bool /*all*/) {

        // write mandatory
        out.write(m_id);
    }

private:
    int16_t m_id;
};
}
#endif // IDDATA_H
