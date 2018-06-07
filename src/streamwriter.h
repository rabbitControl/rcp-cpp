#ifndef RCP_OSTREAM_WRITER_H
#define RCP_OSTREAM_WRITER_H

#include <iostream>
#include <string>

#include "writeable.h"

namespace rcp {

    /*
     * !! streamwriter does not convert endianness - used for debugging
    */
    class StreamWriter : public Writer
    {
    public:
        StreamWriter(std::ostream& os) : os(os) {}
        ~StreamWriter() {}

        virtual void write(const bool& c);
        virtual void write(const char& c);
        virtual void write(const uint8_t& c);
        virtual void write(const int8_t& c);
        virtual void write(const uint16_t& c);
        virtual void write(const int16_t& c);
        virtual void write(const uint32_t& c);
        virtual void write(const int32_t& c);
        virtual void write(const uint64_t& c);
        virtual void write(const int64_t& c);
        virtual void write(const float& c);
        virtual void write(const double& c);
        virtual void write(const std::string& s, bool prefix = true);
        virtual void write(const rcp::Color& s);
        virtual void write(const rcp::IPv4& s);
        virtual void write(const rcp::IPv6& s);
        virtual void write(const char* data, uint32_t length);

    private:
        std::ostream& os;
    };

}

#endif
