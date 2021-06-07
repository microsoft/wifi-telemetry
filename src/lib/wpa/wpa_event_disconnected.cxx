
#include <cinttypes>
#include <cstdio>
#include <iostream>

#include "wifi-telemetry/wpa/wpa_event_disconnected.hpp"

WpaEventDisconnected::WpaEventDisconnected(const wifi_80211_mac& bssid_, WifiDeauthenticationReasonCode reason_code_, bool locally_generated_) :
    WpaEvent(WpaEventType::Disconnected),
    bssid(bssid_),
    reason_code(reason_code_),
    locally_generated(locally_generated_)
{
}
