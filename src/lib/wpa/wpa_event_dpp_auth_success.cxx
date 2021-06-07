
#include "wifi-telemetry/wpa/wpa_event_dpp_auth_success.hpp"

WpaEventDppAuthenticationSucceeded::WpaEventDppAuthenticationSucceeded(int initiator_) :
    WpaEvent(WpaEventType::DppAuthenticationSucceeded),
    initiator(initiator_)
{}
