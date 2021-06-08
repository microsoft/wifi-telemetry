
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "wifi-telemetry/wpa/wpa_command_signal_poll.hpp"

static constexpr char this_command_name[] = "SIGNAL_POLL";

WpaCommandSignalPoll::WpaCommandSignalPoll(void) :
    WpaCommand(this_command_name)
{
    data = name;
}

std::unique_ptr<WpaCommandResponseParser>
WpaCommandSignalPoll::create_response_parser(const std::string_view payload) const
{
    return std::make_unique<WpaCommandSignalPollResponseParser>(payload);
}

WpaCommandSignalPollResponseParser::WpaCommandSignalPollResponseParser(const std::string_view payload_) :
    WpaCommandResponseParser(this_command_name, payload_, {
        { "RSSI=", WpaValueRequired },
        { "NOISE=", WpaValueRequired },
        { "LINKSPEED=", WpaValueRequired },
        { "FREQUENCY=", WpaValueRequired },
        { "CENTER_FRQ1=", WpaValueOptional },
        { "CENTER_FRQ2=", WpaValueOptional },
        { "WIDTH=", WpaValueOptional },
        { "AVG_RSSI=", WpaValueOptional },
        { "AVG_BEACON_RSSI=", WpaValueOptional },
    })
{}

std::shared_ptr<WpaCommandResponse>
WpaCommandSignalPollResponseParser::parse_payload() const
{
    int32_t rssi = static_cast<int32_t>(strtol(properties[0](), nullptr, 10));
    int32_t noise = static_cast<int32_t>(strtol(properties[1](), nullptr, 10));
    int32_t link_speed = static_cast<int32_t>(strtol(properties[2](), nullptr, 10));
    uint32_t frequency = static_cast<uint32_t>(strtoul(properties[3](), nullptr, 10));

    std::shared_ptr<WpaCommandSignalPollResponse> response = std::make_shared<WpaCommandSignalPollResponse>(noise, rssi, link_speed, frequency);

    if (properties[4].value)
        response->center_frequency_1 = static_cast<int32_t>(strtol(properties[4](), nullptr, 10));
    if (properties[5].value)
        response->center_frequency_2 = static_cast<int32_t>(strtol(properties[5](), nullptr, 10));
    if (properties[6].value) {
        std::size_t value_pos = properties[6].value_pos;
        std::size_t value_pos_end = payload.find_first_of('\n', value_pos);
        if (value_pos_end != payload.npos)
            response->channel_width = payload.substr(value_pos, value_pos_end - value_pos);
    }
    if (properties[7].value)
        response->rssi_average = static_cast<int32_t>(strtol(properties[7](), nullptr, 10));
    if (properties[8].value)
        response->rssi_average_beacon = static_cast<int32_t>(strtol(properties[8](), nullptr, 10));

    return response;
}

WpaCommandSignalPollResponse::WpaCommandSignalPollResponse(int32_t noise_, int32_t rssi_, int32_t link_speed_, uint32_t frequency_) :
    noise(noise_),
    rssi(rssi_),
    link_speed(link_speed_),
    frequency(frequency_)
{}
