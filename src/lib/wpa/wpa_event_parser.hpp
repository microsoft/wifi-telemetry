
#ifndef __WPA_EVENT_PARSER_HPP__
#define __WPA_EVENT_PARSER_HPP__

#include <memory>
#include <string_view>

#include "wifi-telemetry/wpa/wpa_event.hpp"

class WpaEventParser
{
public:
    static std::shared_ptr<WpaEvent>
    parse(const char *buffer);
};

#endif //__WPA_EVENT_PARSER_HPP__
