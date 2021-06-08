
#include <string.h>

#include "wifi-telemetry/wpa/wpa_state.hpp"

WpaState
wpa_state_from_string(const char *state)
{
    if (strcmp(state, "DISCONNECTED") == 0)
        return WpaState::Disconnected;
    else if (strcmp(state, "INACTIVE") == 0)
        return WpaState::Inactive;
    else if (strcmp(state, "INTERFACE_DISABLED") == 0)
        return WpaState::InterfaceDisabled;
    else if (strcmp(state, "SCANNING") == 0)
        return WpaState::Scanning;
    else if (strcmp(state, "AUTHENTICATING") == 0)
        return WpaState::Authenticating;
    else if (strcmp(state, "ASSOCIATING") == 0)
        return WpaState::Associating;
    else if (strcmp(state, "ASSOCIATED") == 0)
        return WpaState::Associated;
    else if (strcmp(state, "4WAY_HANDSHAKE") == 0)
        return WpaState::FourWayHandshake;
    else if (strcmp(state, "GROUP_HANDSHAKE") == 0)
        return WpaState::GroupHandshake;
    else if (strcmp(state, "COMPLETED") == 0)
        return WpaState::Completed;
    else
        return WpaState::Unknown;
}
