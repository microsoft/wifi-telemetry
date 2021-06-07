
#ifndef __WPA_EVENT_DPP_AUTH_SUCCESS_HPP__
#define __WPA_EVENT_DPP_AUTH_SUCCESS_HPP__

#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventDppAuthenticationSucceeded : public WpaEvent
{
    WpaEventDppAuthenticationSucceeded(int initiator);
    int initiator;
};

#endif // __WPA_EVENT_DPP_AUTH_SUCCESS_HPP__
