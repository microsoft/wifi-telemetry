
#ifndef __WPA_EVENT_DPP_CHIRP_RECEIVED_HPP__
#define __WPA_EVENT_DPP_CHIRP_RECEIVED_HPP__

#include <cinttypes>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventDppChirpReceived : public WpaEvent
{
    WpaEventDppChirpReceived(int32_t id, const wifi_80211_mac& bssid, unsigned int frequency);

    int32_t id;
    wifi_80211_mac bssid;
    uint32_t frequency;
};

#endif // __WPA_EVENT_DPP_CHIRP_RECEIVED_HPP__
