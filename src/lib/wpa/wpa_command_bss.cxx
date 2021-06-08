
#include <sstream>

#include "wifi-telemetry/wifi/wifi_80211_format_mac.hpp"
#include "wifi-telemetry/wpa/wpa_command_bss.hpp"

static constexpr char this_command_name[] = "BSS";

WpaCommandBss::WpaCommandBss(std::string bssid_) :
    WpaCommand(this_command_name),
    bssid(bssid_)
{
    std::stringstream ss(name);
    ss << " " << bssid;
    data = ss.str();
}

std::unique_ptr<WpaCommandResponseParser>
WpaCommandBss::create_response_parser(const std::string_view payload) const
{
    return std::make_unique<WpaCommandBssResponseParser>(payload);
}

WpaCommandBssResponseParser::WpaCommandBssResponseParser(const std::string_view payload_) :
    WpaCommandResponseParser(this_command_name, payload_, {
        { "id=", WpaValueRequired },
        { "bssid=", WpaValueRequired },
        { "freq=", WpaValueRequired },
        { "beacon_int=", WpaValueRequired },
        { "capabilities=", WpaValueRequired },
        { "qual=", WpaValueRequired },
        { "noise=", WpaValueRequired },
        { "level=", WpaValueRequired },
        { "tsf=", WpaValueRequired },
        { "age=", WpaValueRequired },
        { "ie=", WpaValueRequired },
        { "flags=", WpaValueRequired },
        { "ssid=", WpaValueRequired },
        { "snr=", WpaValueRequired },
        { "est_throughput=", WpaValueRequired },
        { "update_idx=", WpaValueRequired },
        { "beacon_ie=", WpaValueRequired },
    })
{}

std::shared_ptr<WpaCommandResponse>
WpaCommandBssResponseParser::parse_payload(void) const
{
    return nullptr;
}

WpaCommandBss::WpaCommandBss(const wifi_80211_mac& bssid_) :
    WpaCommandBss(wifi_80211_mac_to_string(bssid_))
{}

WpaCommandBssResponse::WpaCommandBssResponse()
{}
