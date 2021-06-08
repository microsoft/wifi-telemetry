
#include "wifi-telemetry/wpa/wpa_event_association_rejected.hpp"

WpaEventAssociationRejected::WpaEventAssociationRejected(WifiStatusCode status_code_) :
    WpaEvent(WpaEventType::AssociationRejected),
    status_code(status_code_)
{
}

WpaEventAssociationRejected::WpaEventAssociationRejected(WifiStatusCode status_code_, const wifi_80211_mac& bssid_) :
    WpaEvent(WpaEventType::AssociationRejected),
    status_code(status_code_),
    bssid(bssid_)
{
}
