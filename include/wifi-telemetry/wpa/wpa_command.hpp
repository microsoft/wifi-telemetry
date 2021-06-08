
#ifndef __WPA_COMMAND_HPP__
#define __WPA_COMMAND_HPP__

#include <cinttypes>
#include <memory>
#include <string.h>
#include <string>
#include <string_view>

#include "wifi-telemetry/wpa/wpa_command_response.hpp"
#include "wifi-telemetry/wpa/wpa_command_response_parser.hpp"

struct WpaCommand : private WpaCommandResponseParserFactory
{
    WpaCommand(const std::string& name);
    virtual ~WpaCommand() = default;

    std::shared_ptr<WpaCommandResponse>
    parse_response(const std::string_view payload) const;

    const std::string name;
    std::string data;
};

#endif //__WPA_COMMAND_HPP__
