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

#ifndef RCP_IP_H
#define RCP_IP_H

#include <ostream>
#include <inttypes.h>

namespace rcp {

    class IPv4 {

    public:
        IPv4() {}
        IPv4(const uint32_t& ip) {
            m_ip = ip;
        }

        void setAddress(const uint32_t& ip) {
            m_ip = ip;
        }
        uint32_t getAddress() const { return m_ip; }

        bool operator==(const IPv4& other) {
            return m_ip == other.getAddress();
        }

        bool operator!=(const IPv4& other) {
            return m_ip != other.getAddress();
        }

    private:
        uint32_t m_ip{0};
    };

    std::string value_to_string(IPv4 value);



    class IPv6 {

    public:
        IPv6() {}
        IPv6(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) {
            m_ip[0] = v1;
            m_ip[1] = v2;
            m_ip[2] = v3;
            m_ip[3] = v4;
        }

        void setAddress(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4) {
            m_ip[0] = v1;
            m_ip[1] = v2;
            m_ip[2] = v3;
            m_ip[3] = v4;
        }
        uint32_t getAddress(const int index) const {
            if (index < 0 || index > 3) {
                return 0;
            }
            return m_ip[index];
        }

        bool operator==(const IPv6& other) {
            return m_ip[0] == other.getAddress(0) &&
                    m_ip[1] == other.getAddress(1) &&
                    m_ip[2] == other.getAddress(2) &&
                    m_ip[3] == other.getAddress(3);
        }

        bool operator!=(const IPv6& other) {
            return m_ip[0] != other.getAddress(0) ||
                    m_ip[1] != other.getAddress(1) ||
                    m_ip[2] != other.getAddress(2) ||
                    m_ip[3] != other.getAddress(3);
        }

    private:
        uint32_t m_ip[4]{0, 0, 0, 0};
    };

    std::string value_to_string(IPv6 value);


    std::ostream& operator<<(std::ostream& out, const IPv4& v);
    std::ostream& operator<<(std::ostream& out, const IPv6& v);

    IPv4& swap_endian(const IPv4 &u);
    IPv6& swap_endian(const IPv6 &u);

    IPv4 readFromStream(std::istream& is, const IPv4& i);
    IPv6 readFromStream(std::istream& is, const IPv6& i);

}

#endif // RCP_IP_H
