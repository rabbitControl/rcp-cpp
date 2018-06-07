#ifndef RCP_COLOR_H
#define RCP_COLOR_H

#include <stdint.h>
#include <ostream>

namespace rcp {

    class Color
    {
    public:
        Color() {}
        Color(uint32_t c) : value(c) {}

        uint32_t getValue() { return value; }
        uint32_t getValue() const { return value; }
        void setValue(uint32_t v) { value = v; }

        Color& operator=(const Color& other) {
            value = other.getValue();
            return *this;
        }

        bool operator==(const Color& other) {
            return value == other.getValue();
        }

    private:
        uint32_t value{};
    };

}

#endif
