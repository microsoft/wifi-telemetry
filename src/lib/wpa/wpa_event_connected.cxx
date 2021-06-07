
#include "wifi-telemetry/wpa/wpa_event_connected.hpp"

WpaEventConnected::WpaEventConnected(const wifi_80211_mac& bssid_) :
    WpaEvent(WpaEventType::Connected),
    bssid(bssid_)
{}
