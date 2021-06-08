
#ifndef __WIFI_DPP_EXCHANGE_HPP__
#define __WIFI_DPP_EXCHANGE_HPP__

#include <chrono>
#include <optional>
#include <string>
#include <unordered_set>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wifi/wifi_common.hpp"
#include "wifi-telemetry/wifi/wifi_dpp.hpp"

struct WifiDppExchange
{
    WifiDppExchange(WifiDppDeviceRole device_role_, WifiDppRole role_) :
        WifiDppExchange(device_role_, role_, std::chrono::system_clock::now())
    {}

    WifiDppExchange(WifiDppDeviceRole device_role_, WifiDppRole role_, std::chrono::time_point<std::chrono::system_clock> timestamp_start_) :
        device_role(device_role_),
        role(role_),
        timestamp_start(timestamp_start_)
    {}

    virtual ~WifiDppExchange(void) = default;

    void
    start(void)
    {
        if (!timestamp_start)
            timestamp_start = std::chrono::system_clock::now();
    }

    void
    stop(void)
    {
        if (timestamp_start && !timestamp_end) {
            timestamp_end = std::chrono::system_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(timestamp_end.value() - timestamp_start.value());
        }
    }

    WifiDppDeviceRole device_role;
    WifiDppRole role;
    WifiDppFailureType failure_type = WifiDppFailureType::None;
    std::optional<std::string> failure_details;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> timestamp_start;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> timestamp_end;
    std::optional<std::chrono::milliseconds> duration;
};

struct WifiDppExchangeEnrollee :
    public WifiDppExchange
{
    WifiDppExchangeEnrollee(WifiDppRole role_) :
        WifiDppExchange(WifiDppDeviceRole::Enrollee, role_)
    {}

    WifiDppExchangeEnrolleeState state = WifiDppExchangeEnrolleeState::Bootstrapping;
    std::unordered_set<uint32_t> chirp_frequencies;
};

struct WifiDppExchangeConfigurator :
    public WifiDppExchange
{
    WifiDppExchangeConfigurator(int32_t peer_id_, const wifi_80211_mac peer_bssid_, uint32_t frequency_, WifiDppRole role_) :
        WifiDppExchange(WifiDppDeviceRole::Configurator, role_),
        peer_id(peer_id_),
        peer_bssid(peer_bssid_),
        frequency(frequency_)
    {}

    WifiDppExchangeConfiguratorState state = WifiDppExchangeConfiguratorState::Bootstrapping;
    int32_t peer_id;
    wifi_80211_mac peer_bssid;
    uint32_t frequency;
};

#endif //__WIFI_DPP_EXCHANGE_HPP__
