#include "rcp/1.0/RcpVersion.h"

namespace rcp {
namespace v10 {

std::ostream& operator<<(std::ostream& out, const RcpVersion& version)
{
    out << RcpVersion::to_string(version);
    return out;
}

}
}
