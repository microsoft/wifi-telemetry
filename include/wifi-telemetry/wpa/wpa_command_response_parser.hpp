
#ifndef __WPA_COMMAND_RESPONSE_PARSER_HPP__
#define __WPA_COMMAND_RESPONSE_PARSER_HPP__

#include <memory>
#include <optional>
#include <string_view>
#include <vector>

#include "wifi-telemetry/wpa/wpa_command_key_value_pair.hpp"
#include "wifi-telemetry/wpa/wpa_command_response.hpp"

struct WpaCommandResponseParser
{
    WpaCommandResponseParser(const std::string& command_name, const std::string_view payload, const std::initializer_list<WpaCommandKeyValuePair> properties);
    virtual ~WpaCommandResponseParser() = default;

    virtual std::shared_ptr<WpaCommandResponse>
    parse(void);

    const std::string command_name;
    const std::string_view payload;

protected:
    virtual std::shared_ptr<WpaCommandResponse>
    parse_payload(void) const = 0;

    bool
    resolve_properties(void);

protected:
    std::vector<WpaCommandKeyValuePair> properties;
    std::optional<bool> properties_resolved_result = std::nullopt;
};

struct WpaCommandResponseParserFactory
{
    virtual std::unique_ptr<WpaCommandResponseParser>
    create_response_parser(const std::string_view payload) const = 0;
};

#endif //__WPA_COMMAND_RESPONSE_PARSER_HPP__
