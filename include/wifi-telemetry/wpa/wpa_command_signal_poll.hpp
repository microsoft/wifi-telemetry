
#ifndef __WPA_COMMAND_SIGNAL_POLL_HPP__
#define __WPA_COMMAND_SIGNAL_POLL_HPP__

#include <cinttypes>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>

#include "wifi-telemetry/wpa/wpa_command.hpp"
#include "wifi-telemetry/wpa/wpa_command_response.hpp"
#include "wifi-telemetry/wpa/wpa_command_response_parser.hpp"

struct WpaCommandSignalPollResponseParser : public WpaCommandResponseParser
{
    WpaCommandSignalPollResponseParser(const std::string_view payload);

    std::shared_ptr<WpaCommandResponse>
    parse_payload(void) const override;
};

struct WpaCommandSignalPollResponse : public WpaCommandResponse
{
    WpaCommandSignalPollResponse(int32_t noise, int32_t rssi, int32_t link_speed, uint32_t frequency);

    int32_t noise;
    int32_t rssi;
    int32_t link_speed;
    uint32_t frequency;
    std::optional<std::string> channel_width;
    std::optional<int32_t> center_frequency_1 = std::nullopt;
    std::optional<int32_t> center_frequency_2 = std::nullopt;
    std::optional<int32_t> rssi_average = std::nullopt;
    std::optional<int32_t> rssi_average_beacon = std::nullopt;
};

struct WpaCommandSignalPoll : public WpaCommand
{
    WpaCommandSignalPoll(void);

private:
    std::unique_ptr<WpaCommandResponseParser>
    create_response_parser(const std::string_view payload) const override;
};

#endif // __WPA_COMMAND_SIGNAL_POLL_HPP__
