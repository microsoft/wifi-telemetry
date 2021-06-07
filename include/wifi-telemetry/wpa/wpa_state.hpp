
#ifndef __WPA_STATE_HPP__
#define __WPA_STATE_HPP__

#include <cinttypes>

enum class WpaState : uint32_t
{
    Unknown = 0,
    Disconnected = 1,
    InterfaceDisabled = 2,
    Inactive = 3,
    Scanning = 4,
    Authenticating = 5,
    Associating = 6,
    Associated = 7,
    FourWayHandshake = 8,
    GroupHandshake = 9,
    Completed = 10
};

WpaState
wpa_state_from_string(const char* state);

#endif //__WPA_STATE_HPP__
