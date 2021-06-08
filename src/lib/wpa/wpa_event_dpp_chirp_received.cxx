
#include <cinttypes>
#include <cstdio>
#include <iostream>

#include "wifi-telemetry/wpa/wpa_event_dpp_chirp_received.hpp"

WpaEventDppChirpReceived::WpaEventDppChirpReceived(int32_t id_, const wifi_80211_mac& bssid_, uint32_t frequency_) :
    WpaEvent(WpaEventType::DppChirpReceived),
    id(id_),
    bssid(bssid_),
    frequency(frequency_)
{
}
