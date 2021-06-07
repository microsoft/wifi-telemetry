
#ifndef __WPA_EVENT_DPP_FAILURE_HPP__
#define __WPA_EVENT_DPP_FAILURE_HPP__

#include <string>

#include "wifi-telemetry/wifi/wifi_dpp.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventDppFailure : public WpaEvent
{
    WpaEventDppFailure(const std::string& failure_details = "");
    WpaEventDppFailure(WifiDppFailureType failure_type, const std::string& failure_details = "");

    WifiDppFailureType failure_type;
    const std::string failure_details;

protected:
    WpaEventDppFailure(WpaEventType type, WifiDppFailureType failure_type, const std::string& failure_details = "");
};

#endif // __WPA_EVENT_DPP_FAILURE_HPP__
