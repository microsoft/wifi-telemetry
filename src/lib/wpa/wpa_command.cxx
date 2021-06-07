
#include <iostream>

#include "wifi-telemetry/wpa/wpa_command.hpp"

WpaCommand::WpaCommand(const std::string& name_) :
    name(name_)
{}

std::shared_ptr<WpaCommandResponse>
WpaCommand::parse_response(const std::string_view payload) const
{
    std::unique_ptr<WpaCommandResponseParser> parser = create_response_parser(payload);
    return parser->parse();
}
