#include "InfoData.h"

namespace rcp {
namespace v10 {

std::ostream& operator<<(std::ostream& out, const InfoData& info)
{
    out << "protocol: " << info.protocolVersion() << ", handshake: " << info.handshakeVersion();
    if (!info.applicationId().empty())
    {
        out << ", application id: " << info.applicationId();
    }
    if (!info.applicationVersion().empty())
    {
        out << ", application version: " << info.applicationVersion();
    }
    return out;
}

}
}
