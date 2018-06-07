/**
  */


#include "streamwriter.h"

namespace rcp {

    void StreamWriter::write(const bool& c) {
        os << c;
    }

    void StreamWriter::write(const char& c) {
        os << c;
    }

    void StreamWriter::write(const uint8_t& c) {
        os << c;
    }
    void StreamWriter::write(const int8_t& c) {
        os << c;
    }


    void StreamWriter::write(const uint16_t& c) {
        os << c;
    }
    void StreamWriter::write(const int16_t& c) {
        os << c;
    }


    void StreamWriter::write(const uint32_t& c) {
        os << c;
    }
    void StreamWriter::write(const int32_t& c) {
        os << c;
    }


    void StreamWriter::write(const uint64_t& c) {
        os << c;
    }
    void StreamWriter::write(const int64_t& c) {
        os << c;
    }


    void StreamWriter::write(const float& c) {
        os << c;
    }
    void StreamWriter::write(const double& c) {
        os << c;
    }


    void StreamWriter::write(const std::string& s, bool prefix) {
        if (prefix) {
            write(static_cast<uint32_t>(s.length()));
        }
        os << s;
    }


    void StreamWriter::write(const rcp::Color& s) {
        os << s.getValue();
    }


    void StreamWriter::write(const rcp::IPv4& s) {
        os << static_cast<uint32_t>(s.getIp());
    }

    void StreamWriter::write(const rcp::IPv6& s) {
        for (int i=0; i<4; i++) {
            os << static_cast<uint32_t>(s.getIp().__u6_addr.__u6_addr32[i]);
        }
    }


    void StreamWriter::write(const char* data, uint32_t length) {
        os.write(data, length);
    }

} // namespace
