#include "packet.h"
#include "streamwriter.h"

namespace rcp {

    Packet::~Packet() {}

    std::ostream& operator<<(std::ostream& out, Packet& packet) {
        StreamWriter w(out);
        packet.write(w, false);
        return out;
    }
}
