
#ifndef __WIFI_TELEMETRY_80211_HPP__
#define __WIFI_TELEMETRY_80211_HPP__

enum class WifiConnectionAttemptResult
{
    Succeeded = 0,
    Unspecified = 1,
    AuthenticationRejected = 2,
    AssociationRejected = 3,
    NetworkNotFound = 4,
};

#endif // __WIFI_TELEMETRY_80211_HPP__
