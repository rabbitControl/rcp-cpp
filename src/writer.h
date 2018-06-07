#ifndef WRITER_H
#define WRITER_H

#include <inttypes.h>

#include "color.h"
#include "range.h"
#include "ip.h"

namespace rcp {

    class Writer {
    public:
        virtual ~Writer();

        virtual void write(const bool& c) = 0;
        virtual void write(const char& c) = 0;
        virtual void write(const uint8_t& c) = 0;
        virtual void write(const int8_t& c) = 0;
        virtual void write(const uint16_t& c) = 0;
        virtual void write(const int16_t& c) = 0;
        virtual void write(const uint32_t& c) = 0;
        virtual void write(const int32_t& c) = 0;
        virtual void write(const uint64_t& c) = 0;
        virtual void write(const int64_t& c) = 0;
        virtual void write(const float& c) = 0;
        virtual void write(const double& c) = 0;
        virtual void write(const std::string& s, bool prefix = true) = 0;
        virtual void write(const rcp::Color& s) = 0;
        virtual void write(const rcp::IPv4& s) = 0;
        virtual void write(const rcp::IPv6& s) = 0;
        virtual void write(const char* data, uint32_t length) = 0;

        template<typename T>
        void write(const Range<T>& c) {
            write(c.value1());
            write(c.value2());
        }

        void writeTinyString(const std::string& s){
            if (s.length() >= UINT8_MAX) {
                write(UINT8_MAX);
                write(s.substr(0, UINT8_MAX), false); // write without length-prefix
            } else {
                write(static_cast<uint8_t>(s.length()));
                write(s, false); // write without length-prefix
            }
        }
        void writeShortString(const std::string& s){
            if (s.length() >= UINT16_MAX) {
                write(UINT16_MAX);
                write(s.substr(0, UINT16_MAX), false); // write without length-prefix
            } else {
                write(static_cast<uint16_t>(s.length()));
                write(s, false); // write without length-prefix
            }
        }
        void writeString(const std::string& s){
            if (s.length() >= UINT32_MAX) {
                write(UINT32_MAX);
                write(s.substr(0, UINT32_MAX), false); // write without length-prefix
            } else {
                write(static_cast<uint32_t>(s.length()));
                write(s, false); // write without length-prefix
            }
        }
    };


    template<typename T>
    Writer& operator<<(Writer& out, const T& c)
    {
        out.write(c);
        return out;
    }

    template<typename T>
    Writer& operator<<(Writer& out, const Range<T>& c)
    {
        out.write(c.value1());
        out.write(c.value2());
        return out;
    }

}

#endif // WRITER_H
