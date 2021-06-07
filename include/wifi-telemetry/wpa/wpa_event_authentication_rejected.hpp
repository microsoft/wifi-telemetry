
#ifndef __WPA_EVENT_AUTHENTICATION_REJECTED_HPP__
#define __WPA_EVENT_AUTHENTICATION_REJECTED_HPP__

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventAuthenticationRejected : public WpaEvent
{
    WpaEventAuthenticationRejected(const wifi_80211_mac& bssid, WifiStatusCode status_code, WifiAuthenticationType authentication_type);

    const wifi_80211_mac bssid;
    const WifiStatusCode status_code;
    const WifiAuthenticationType authentication_type;
};

#endif // __WPA_EVENT_AUTHENTICATION_REJECTED_HPP__
