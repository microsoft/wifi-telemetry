
#include <iostream>

#include "wifi-telemetry/wpa/wpa_command_response_parser.hpp"

WpaCommandResponseParser::WpaCommandResponseParser(const std::string& command_name_, const std::string_view payload_, const std::initializer_list<WpaCommandKeyValuePair> properties_) :
    command_name(command_name_),
    payload(payload_),
    properties(properties_)
{}

std::shared_ptr<WpaCommandResponse>
WpaCommandResponseParser::parse(void)
{
    return resolve_properties()
        ? parse_payload()
        : nullptr;
}

bool
WpaCommandResponseParser::resolve_properties(void)
{
    if (properties_resolved_result.has_value())
        return properties_resolved_result.value();

    for (auto& property : properties) {
        if (!property.resolve(payload) && property.is_required) {
            std::cerr << "invalid " << command_name << " response (" << property.key << " not found)" << std::endl;
            properties_resolved_result = false;
            return false;
        }
    }

    properties_resolved_result = true;
    return true;
}
