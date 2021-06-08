
#ifndef __WPA_EVENT_DISCONNECTED_HPP__
#define __WPA_EVENT_DISCONNECTED_HPP__

#include <array>
#include <cinttypes>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventDisconnected : public WpaEvent
{
    WpaEventDisconnected(const wifi_80211_mac& bssid_, WifiDeauthenticationReasonCode reason_code_, bool locally_generated);

    const wifi_80211_mac bssid;
    const WifiDeauthenticationReasonCode reason_code;
    bool locally_generated = false;
};

#endif // __WPA_EVENT_DISCONNECTED_HPP__
