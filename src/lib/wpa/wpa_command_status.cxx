
#include <algorithm>

#include "wifi-telemetry/wifi/wifi_80211_format_mac.hpp"
#include "wifi-telemetry/wpa/wpa_command_status.hpp"

static constexpr char this_command_name[] = "STATUS";

WpaCommandStatus::WpaCommandStatus(void) :
    WpaCommand(this_command_name)
{
    data = name;
}

std::unique_ptr<WpaCommandResponseParser>
WpaCommandStatus::create_response_parser(const std::string_view payload) const
{
    return std::make_unique<WpaCommandStatusResponseParser>(payload);
}

WpaCommandStatusResponseParser::WpaCommandStatusResponseParser(const std::string_view payload_) :
    WpaCommandResponseParser(this_command_name, payload_, {
        { "wpa_state=", WpaValueRequired },
        { "ssid=", WpaValueOptional },
        { "bssid=", WpaValueOptional },
        { "pairwise_cipher=", WpaValueOptional },
        { "group_cipher=", WpaValueOptional },
        { "key_mgmt=", WpaValueOptional },
        { "wifi_generation=", WpaValueOptional },
        { "freq=", WpaValueOptional },
    })
{}

std::shared_ptr<WpaCommandResponse>
WpaCommandStatusResponseParser::parse_payload(void) const
{
    std::string state = properties[0]();
    state.erase(state.find_first_of('\n'));

    auto response = std::make_shared<WpaCommandStatusResponse>(wpa_state_from_string(state.c_str()));

    if (properties[1].value) {
        std::string ssid = properties[1]();
        ssid.erase(ssid.find_first_of('\n'));
        response->ssid = std::move(ssid);
    }

    if (properties[2].value) {
        std::string bssid = properties[2]();
        bssid.erase(bssid.find_first_of('\n'));
        response->bssid = wifi_80211_mac_from_string(bssid);
    }

    if (properties[3].value) {
        std::string pairwise_cipher = properties[3]();
        pairwise_cipher.erase(pairwise_cipher.find_first_of('\n'));
        response->pairwise_cipher = std::move(pairwise_cipher);
    }

    if (properties[4].value) {
        std::string group_cipher = properties[4]();
        group_cipher.erase(group_cipher.find_first_of('\n'));
        response->group_cipher = std::move(group_cipher);
    }

    if (properties[5].value) {
        std::string key_mgmt = properties[5]();
        key_mgmt.erase(key_mgmt.find_first_of('\n'));
        response->key_mgmt = std::move(key_mgmt);
    }

    if (properties[6].value)
        response->wifi_generation = static_cast<uint32_t>(strtoul(properties[6](), nullptr, 10));

    if (properties[7].value)
        response->frequency = static_cast<uint32_t>(strtoul(properties[7](), nullptr, 10));

    return response;
}

WpaCommandStatusResponse::WpaCommandStatusResponse(WpaState state_) :
    state(state_)
{}
