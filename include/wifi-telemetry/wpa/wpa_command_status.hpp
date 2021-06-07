
#ifndef __WPA_COMMAND_STATUS_HPP__
#define __WPA_COMMAND_STATUS_HPP__

#include <cinttypes>
#include <string>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_command.hpp"
#include "wifi-telemetry/wpa/wpa_state.hpp"

struct WpaCommandStatusResponseParser : public WpaCommandResponseParser
{
    WpaCommandStatusResponseParser(const std::string_view payload);

    std::shared_ptr<WpaCommandResponse>
    parse_payload(void) const override;
};

struct WpaCommandStatusResponse : public WpaCommandResponse
{
    WpaCommandStatusResponse(void) = default;
    WpaCommandStatusResponse(WpaState state_);

    WpaState state = WpaState::Unknown;
    std::optional<wifi_80211_mac> bssid;
    std::optional<uint32_t> frequency;
    std::optional<std::string> ssid;
    std::optional<std::string> passphrase;
    std::optional<std::string> id_string;
    std::optional<std::string> mode;
    std::optional<uint32_t> wifi_generation;
    std::optional<int32_t> sae_group;
    std::optional<std::string> ip_address;
    std::optional<std::string> p2p_address;
    std::optional<std::string> address;
    std::optional<int32_t> hs20;
    std::optional<std::string> provisioning_sp;
    std::optional<std::string> home_sp;
    std::optional<std::string> sp_type;
    std::optional<std::string> eap_session_id;
    std::optional<std::string> uuid;
    std::optional<bool> ieee80211ac;
    std::optional<std::string> pairwise_cipher;
    std::optional<std::string> group_cipher;
    std::optional<std::string> key_mgmt;
    std::optional<int32_t> pmf;
    std::optional<std::string> mgmt_group_cipher;
};

struct WpaCommandStatus : public WpaCommand
{
    WpaCommandStatus(void);

private:
    std::unique_ptr<WpaCommandResponseParser>
    create_response_parser(const std::string_view payload) const override;
};

#endif // __WPA_COMMAND_STATUS_HPP__
