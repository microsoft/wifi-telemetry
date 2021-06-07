
#ifndef __WPA_COMMAND_BSS_HPP__
#define __WPA_COMMAND_BSS_HPP__

#include <cinttypes>
#include <string>
#include <vector>

#include "wifi-telemetry/wifi/wifi_80211.hpp"
#include "wifi-telemetry/wpa/wpa_command.hpp"
#include "wifi-telemetry/wpa/wpa_command_response_parser.hpp"

struct WpaCommandBssResponseParser : public WpaCommandResponseParser
{
    WpaCommandBssResponseParser(const std::string_view payload);

    std::shared_ptr<WpaCommandResponse>
    parse_payload(void) const override;
};

struct WpaCommandBssResponse : public WpaCommandResponse
{
    WpaCommandBssResponse();

    wifi_80211_mac bssid;
    uint32_t id;
    uint32_t capabilities;
    uint32_t update_index;
    uint64_t tsf;
    int32_t frequency;
    int32_t beacon_int;
    int32_t quality;
    int32_t snr;
    int32_t estimated_throughput;
    int32_t age;
    std::string flags;
    std::string ssid;
    std::vector<uint8_t> ie;
    std::vector<uint8_t> beacon_ie;
};

struct WpaCommandBss : public WpaCommand
{
    WpaCommandBss(std::string bssid_);
    WpaCommandBss(const wifi_80211_mac& bssid_);

    std::string bssid;

private:
    std::unique_ptr<WpaCommandResponseParser>
    create_response_parser(const std::string_view payload) const override;
};

#endif // __WPA_COMMAND_BSS_HPP__
