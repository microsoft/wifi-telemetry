
#ifndef __WPA_EVENT_DPP_AUTH_INIT_FAILURE_HPP__
#define __WPA_EVENT_DPP_AUTH_INIT_FAILURE_HPP__

#include <string>

#include "wifi-telemetry/wpa/wpa_event_dpp_failure.hpp"

struct WpaEventDppAuthenticationInitFailure : public WpaEventDppFailure
{
    WpaEventDppAuthenticationInitFailure(void);
};

#endif // __WPA_EVENT_DPP_AUTH_INIT_FAILURE_HPP__
