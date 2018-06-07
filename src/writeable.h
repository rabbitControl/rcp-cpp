#ifndef RCP_WRITEABLE_H
#define RCP_WRITEABLE_H

#include <string>

#include "option.h"
#include "writer.h"

namespace rcp {

    class Writeable {
    public:
        virtual ~Writeable();
        virtual void write(Writer& out, bool all) = 0;
    };

    Writer& operator<<(Writer& out, Writeable& Writeable);

    typedef std::shared_ptr<Writeable> WriteablePtr;
}

#endif // RCP_WRITEABLE_H
