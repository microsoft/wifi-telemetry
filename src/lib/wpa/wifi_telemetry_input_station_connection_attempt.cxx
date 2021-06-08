
#include <limits>

#include "wifi-telemetry/wifi/wifi_80211_format_mac.hpp"
#include "wifi-telemetry/wpa/wifi_telemetry_input_station_connection_attempt.hpp"

WifiTelemetryInputStationConnectionAttempt::WifiTelemetryInputStationConnectionAttempt(
    const WifiConnectionAttemptResult result,
    const std::optional<wifi_80211_mac> bssid,
    const std::shared_ptr<WpaCommandSignalPollResponse> signal_poll,
    const std::shared_ptr<WpaCommandStatusResponse> status,
    const WifiStatusCode status_code,
    const WifiAuthenticationType authentication_type) :
    m_result(result),
    m_bssid(wifi_80211_mac_to_string(bssid.value_or(wifi_80211_any_address))),
    m_signal_poll(signal_poll ? signal_poll : std::make_shared<WpaCommandSignalPollResponse>(0, 0, 0, 0)),
    m_status(status ? status : std::make_shared<WpaCommandStatusResponse>()),
    m_status_code(status_code),
    m_authentication_type(WifiAuthenticationTypeToString(authentication_type))
{
    // sanitize all fields used for telemetry event(s) to defaults

    if (!m_status->wifi_generation)
        m_status->wifi_generation = 0;

    if (!m_status->key_mgmt)
        m_status->key_mgmt = "";

    if (!m_status->pairwise_cipher)
        m_status->pairwise_cipher = "";

    if (!m_status->group_cipher)
        m_status->group_cipher = "";

    if (!m_status->ssid)
        m_status->ssid = "";
}

WifiTelemetryInputStationConnectionAttempt
WifiTelemetryInputStationConnectionAttempt::unspecified(void)
{
    return {
        WifiConnectionAttemptResult::Unspecified,
        std::nullopt,
        nullptr,
        nullptr,
        WifiStatusCode::Success,
        WifiAuthenticationType::Unknown
    };
}

WifiTelemetryInputStationConnectionAttempt
WifiTelemetryInputStationConnectionAttempt::succeeded(const wifi_80211_mac& bssid, const std::shared_ptr<WpaCommandSignalPollResponse> signal_poll, const std::shared_ptr<WpaCommandStatusResponse> status)
{
    return {
        WifiConnectionAttemptResult::Succeeded,
        bssid,
        signal_poll,
        status,
        WifiStatusCode::Success,
        WifiAuthenticationType::Unknown
    };
}

WifiTelemetryInputStationConnectionAttempt
WifiTelemetryInputStationConnectionAttempt::association_rejected(const std::optional<wifi_80211_mac>& bssid, const WifiStatusCode status_code)
{
    return {
        WifiConnectionAttemptResult::AssociationRejected,
        bssid,
        nullptr,
        nullptr,
        status_code,
        WifiAuthenticationType::Unknown
    };
}

WifiTelemetryInputStationConnectionAttempt
WifiTelemetryInputStationConnectionAttempt::authentication_rejected(const wifi_80211_mac& /* bssid */, const WifiStatusCode status_code, const WifiAuthenticationType authentication_type)
{
    return {
        WifiConnectionAttemptResult::AuthenticationRejected,
        std::nullopt,
        nullptr,
        nullptr,
        status_code,
        authentication_type
    };
}

WifiTelemetryInputStationConnectionAttempt
WifiTelemetryInputStationConnectionAttempt::network_not_found(void)
{
    return {
        WifiConnectionAttemptResult::NetworkNotFound,
        std::nullopt,
        nullptr,
        nullptr,
        WifiStatusCode::Success,
        WifiAuthenticationType::Unknown
    };
}

uint32_t
WifiTelemetryInputStationConnectionAttempt::result(void) const
{
    return uint32_t(m_result);
}

const char*
WifiTelemetryInputStationConnectionAttempt::bssid(void) const
{
    return m_bssid.c_str();
}

const char*
WifiTelemetryInputStationConnectionAttempt::ssid(void) const
{
    return m_status->ssid.value().c_str();
}

int32_t
WifiTelemetryInputStationConnectionAttempt::noise(void) const
{
    return m_signal_poll->noise;
}

int32_t
WifiTelemetryInputStationConnectionAttempt::rssi(void) const
{
    return m_signal_poll->rssi;
}

int32_t
WifiTelemetryInputStationConnectionAttempt::link_speed(void) const
{
    return m_signal_poll->link_speed;
}

uint32_t
WifiTelemetryInputStationConnectionAttempt::frequency(void) const
{
    return m_signal_poll->frequency;
}

uint32_t
WifiTelemetryInputStationConnectionAttempt::wifi_generation(void) const
{
    return m_status->wifi_generation.value();
}

const char*
WifiTelemetryInputStationConnectionAttempt::key_mgmt(void) const
{
    return m_status->key_mgmt.value().c_str();
}

const char*
WifiTelemetryInputStationConnectionAttempt::pairwise_cipher(void) const
{
    return m_status->pairwise_cipher.value().c_str();
}

const char*
WifiTelemetryInputStationConnectionAttempt::group_cipher(void) const
{
    return m_status->group_cipher.value().c_str();
}

uint16_t
WifiTelemetryInputStationConnectionAttempt::status_code(void) const
{
    switch (m_result) {
        case WifiConnectionAttemptResult::Succeeded:
        case WifiConnectionAttemptResult::AssociationRejected:
        case WifiConnectionAttemptResult::AuthenticationRejected:
            return uint16_t(m_status_code);
        default:
            return std::numeric_limits<uint16_t>::max();
    }
}

const char*
WifiTelemetryInputStationConnectionAttempt::authentication_type(void) const
{
    return m_authentication_type.c_str();
}
