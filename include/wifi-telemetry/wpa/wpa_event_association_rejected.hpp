
#ifndef __WPA_EVENT_ASSOCIATION_REJECTED_HPP__
#define __WPA_EVENT_ASSOCIATION_REJECTED_HPP__

#include <optional>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventAssociationRejected : public WpaEvent
{
    WpaEventAssociationRejected(WifiStatusCode status_code);
    WpaEventAssociationRejected(WifiStatusCode status_code, const wifi_80211_mac& bssid);

    const WifiStatusCode status_code;
    const std::optional<wifi_80211_mac> bssid = std::nullopt;
};

#endif // __WPA_EVENT_ASSOCIATION_REJECTED_HPP__
