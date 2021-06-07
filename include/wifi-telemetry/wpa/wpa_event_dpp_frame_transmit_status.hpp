
#ifndef __WPA_EVENT_DPP_FRAME_TRANSMIT_STATUS_HPP__
#define __WPA_EVENT_DPP_FRAME_TRANSMIT_STATUS_HPP__

#include <cinttypes>
#include <string>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventDppFrameTransmitStatus : public WpaEvent
{
    WpaEventDppFrameTransmitStatus(const wifi_80211_mac& destination_bssid, unsigned int frequency, const std::string& status);

    wifi_80211_mac destination_bssid;
    uint32_t frequency;
    std::string status;
};

#endif // __WPA_EVENT_DPP_FRAME_TRANSMIT_STATUS_HPP__
