#include "writeable.h"

namespace rcp {

    Writeable::~Writeable() {}

    Writer& operator<<(Writer& out, Writeable& writeable) {
        writeable.write(out, false);
        return out;
    }
}
