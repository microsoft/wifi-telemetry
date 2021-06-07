
#include "wifi-telemetry/wpa/wpa_event_dpp_frame_transmit_status.hpp"

WpaEventDppFrameTransmitStatus::WpaEventDppFrameTransmitStatus(const wifi_80211_mac& destination_bssid_, unsigned int frequency_, const std::string& status_) :
    WpaEvent(WpaEventType::DppFrameTransmitStatus),
    destination_bssid(destination_bssid_),
    frequency(frequency_),
    status(status_)
{}
