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

#ifndef VERSIONDATA_H
#define VERSIONDATA_H

#include "writeable.h"
#include "stream_tools.h"

namespace rcp {

    class InfoData;
    typedef std::shared_ptr<InfoData> InfoDataPtr;


    class InfoData : public Writeable {

    public:
        //----------------------------------------
        // parser
        static InfoDataPtr parse(std::istream& is) {

            // read mandatory
            InfoDataPtr info_data = std::make_shared<InfoData>(readTinyString(is));

            // read options
            // read options
            while(!is.eof()) {

                // read option prefix
                infodata_options_t info_option = static_cast<infodata_options_t>(is.get());

                if (info_option == TERMINATOR) {
                    break;
                }

                if (is.eof() || is.fail()) {
                    break;
                }

                switch (info_option) {
                    case INFODATA_OPTIONS_APPLICATIONID:
                        info_data->setApplicationId(readTinyString(is));
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
            out.writeTinyString(m_version);

            // write optionals
            if (all || !m_applicationId.empty()) {
                out.write(static_cast<char>(INFODATA_OPTIONS_APPLICATIONID));
                out.writeTinyString(m_applicationId);
            }

            // terminator
            out.write(static_cast<char>(TERMINATOR));
        }

    private:
        std::string m_version;
        std::string m_applicationId;
    };
}

#endif // VERSIONDATA_H
