#include "ParameterClientState.h"

#include <ostream>

namespace rcp {
namespace v10 {

std::string to_string(ClientState state)
{
    switch (state)
    {
    case Unknown: return "Unknown";
    case Disconnected: return "Disconnected";
    case Connected: return "Connected";
    case Error: return "Error";
    case Handshake: return "Handshake";
    case Initialize: return "Initialize";
    case FullyInitialized: return "FullyInitialized";
    }

    return "Unknown State: " + std::to_string(state);
}


std::ostream& operator<<(std::ostream& out, ClientState state)
{
    out << to_string(state);
    return out;
}


}
}
