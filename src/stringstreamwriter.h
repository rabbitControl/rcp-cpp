#ifndef STRINGSTREAMWRITER_H
#define STRINGSTREAMWRITER_H

#include <sstream>

#include "writeable.h"

namespace rcp {

    /*
     * StringStreamWriter writes big-endian to the stream converting if needed
    */
    class StringStreamWriter : public Writer
    {
    public:
        StringStreamWriter() {}

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


        size_t getSize() {
            return buffer.str().size();
        }

        std::stringstream& getBuffer() {
            return buffer;
        }

        void dump() {
            size_t len = buffer.str().size();
            char data[len];

            buffer.seekg(0);
            buffer.get(data, len);

            for (size_t i=0; i<len; i++) {
                printf("0x%02X ", static_cast<uint8_t>(data[i]));
            }
            printf("\n");
        }

        void clear() { buffer.str(""); }

    private:
        std::stringstream buffer;
    };
}


#endif // STRINGSTREAMWRITER_H
