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

#ifndef RCP_V010_INFODATA_H
#define RCP_V010_INFODATA_H

#include "../Writeable.h"

#include "types.h"
#include "ParameterParser.h"
#include "ParameterWriter.h"

namespace rcp {
namespace v010 {

class InfoData;
using InfoDataPtr = std::shared_ptr<InfoData>;


class InfoData
    : public rcp::Writeable
{

public:
    //----------------------------------------
    // parser
    static InfoDataPtr parse(std::istream& is)
    {
        // read mandatory
        InfoDataPtr info_data = std::make_shared<InfoData>(ParameterParser::readTinyString(is));

        // read options
        char ch;
        while(is.get(ch))
        {
            // read option prefix
            infodata_options_t info_option = static_cast<infodata_options_t>(ch);

            if (info_option == TERMINATOR)
            {
                break;
            }

            switch (info_option)
            {
            case INFODATA_OPTIONS_APPLICATIONID:
                info_data->setApplicationId(ParameterParser::readTinyString(is));
                break;

            default:
                //  NOTE: invalid option!
                break;
            }
        }

        return info_data;
    }

    static inline InfoDataPtr create(const std::string& version, const std::string& applicationId) {
        return std::make_shared<InfoData>(version, applicationId);
    }


    //----------------------------------------
    //
    InfoData(const std::string& version, const std::string& appId = "") :
        m_version(version)
        , m_applicationId(appId)
    {}

    ~InfoData() {}


    //----------------------------------------
    // version
    void setVersion(const std::string& version) {
        m_version = version;
    }

    std::string getVersion() {
        return m_version;
    }

    //----------------------------------------
    // application id
    void setApplicationId(const std::string& appid) {
        m_applicationId = appid;
    }

    std::string getApplicationId() { return m_applicationId; }

    //----------------------------------------
    // interface Writeable
    virtual void write(Writer& out, bool all) {

        // write mandatory
        ParameterWriter::writeTinyString(out, m_version);

        // write optionals
        if (all || !m_applicationId.empty()) {
            out.write(static_cast<char>(INFODATA_OPTIONS_APPLICATIONID));
            ParameterWriter::writeTinyString(out, m_applicationId);
        }

        // terminator
        out.write(static_cast<char>(TERMINATOR));
    }

private:
    std::string m_version;
    std::string m_applicationId;
};

}
}


#endif // INFODATA_H
