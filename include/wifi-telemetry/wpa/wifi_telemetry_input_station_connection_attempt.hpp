
#ifndef __WIFI_TELEMETRY_INPUT_STATION_CONNECTION_ATTEMPT_HPP__
#define __WIFI_TELEMETRY_INPUT_STATION_CONNECTION_ATTEMPT_HPP__

#include <memory>
#include <optional>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wifi/wifi_telemetry_80211.hpp"
#include "wifi-telemetry/wpa/wpa_command_signal_poll.hpp"
#include "wifi-telemetry/wpa/wpa_command_status.hpp"

/**
 * @brief Helper class that sanitizes the connection_attempt telemetry event
 * inputs. 
 *
 * Some of the input data is optional; this class checks such inputs for their
 * existence/validity, and if missing/invalid, assigns defaults or appropriate
 * values that the telemetry system understands as being invalid.
 */
class WifiTelemetryInputStationConnectionAttempt
{
public:
    /**
     * @brief Factory methods for creating instances of the correspoding
     * result-type. 
     *
     * These ensure that invalid objects are impossible to create. For example,
     * if the connection attempt result was 'AssociationRejected' ALL required
     * parameters must be specified for the instance to be created since no
     * public constructor exists that allows construction without the required
     * arguments.
     */

    static WifiTelemetryInputStationConnectionAttempt
    unspecified(void);

    static WifiTelemetryInputStationConnectionAttempt
    succeeded(const wifi_80211_mac& bssid, const std::shared_ptr<WpaCommandSignalPollResponse> signal_poll, const std::shared_ptr<WpaCommandStatusResponse> status);

    static WifiTelemetryInputStationConnectionAttempt
    association_rejected(const std::optional<wifi_80211_mac>& bssid, const WifiStatusCode status_code);

    static WifiTelemetryInputStationConnectionAttempt
    authentication_rejected(const wifi_80211_mac& bssid, const WifiStatusCode status_code, const WifiAuthenticationType authentication_type);

    static WifiTelemetryInputStationConnectionAttempt
    network_not_found(void);

    uint32_t
    result(void) const;

    const char*
    bssid(void) const;

    const char*
    ssid(void) const;

    int32_t
    noise(void) const;

    int32_t
    rssi(void) const;

    int32_t
    link_speed(void) const;

    uint32_t
    frequency(void) const;

    uint32_t
    wifi_generation(void) const;

    const char*
    key_mgmt(void) const;

    const char*
    pairwise_cipher(void) const;

    const char*
    group_cipher(void) const;

    uint16_t
    status_code(void) const;

    const char*
    authentication_type(void) const;

private:
    WifiTelemetryInputStationConnectionAttempt(
        const WifiConnectionAttemptResult result,
        const std::optional<wifi_80211_mac> bssid,
        const std::shared_ptr<WpaCommandSignalPollResponse> signal_poll,
        const std::shared_ptr<WpaCommandStatusResponse> status,
        const WifiStatusCode status_code,
        const WifiAuthenticationType authentication_type);

    const WifiConnectionAttemptResult m_result;
    const std::string m_bssid;
    const std::shared_ptr<WpaCommandSignalPollResponse> m_signal_poll;
    const std::shared_ptr<WpaCommandStatusResponse> m_status;
    const WifiStatusCode m_status_code;
    const std::string m_authentication_type;
};

#endif //__WIFI_TELEMETRY_INPUT_STATION_CONNECTION_ATTEMPT_HPP__
