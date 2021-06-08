
#ifndef __WPA_EVENT_CONNECTED_HPP__
#define __WPA_EVENT_CONNECTED_HPP__

#include <array>
#include <cinttypes>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventConnected : public WpaEvent
{
    WpaEventConnected(const wifi_80211_mac& bssid);
    const wifi_80211_mac bssid;
};

#endif // __WPA_EVENT_CONNECTED_HPP__
