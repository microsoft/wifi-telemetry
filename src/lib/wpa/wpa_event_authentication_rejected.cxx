
#include "wifi-telemetry/wpa/wpa_event_authentication_rejected.hpp"

WpaEventAuthenticationRejected::WpaEventAuthenticationRejected(const wifi_80211_mac& bssid_, WifiStatusCode status_code_, WifiAuthenticationType authentication_type_) :
    WpaEvent(WpaEventType::AuthenticationRejected),
    bssid(bssid_),
    status_code(status_code_),
    authentication_type(authentication_type_)
{
}
