
#include "wifi-telemetry/wpa/wpa_event_dpp_auth_init_failure.hpp"

WpaEventDppAuthenticationInitFailure::WpaEventDppAuthenticationInitFailure(void) :
    WpaEventDppFailure(WpaEventType::DppAuthenticationInitFailure, WifiDppFailureType::AuthenticationTimeout)
{}
