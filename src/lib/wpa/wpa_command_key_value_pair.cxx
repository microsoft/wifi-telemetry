
#include <string.h>

#include "wifi-telemetry/wpa/wpa_command_key_value_pair.hpp"

WpaCommandKeyValuePair::WpaCommandKeyValuePair(const char* key_, bool is_required_ = WpaValueOptional) :
    key(key_),
    key_length(strlen(key)),
    value(std::nullopt),
    is_required(is_required_)
{}

const char*
WpaCommandKeyValuePair::operator()(void) const
{
    return value.value();
}

bool
WpaCommandKeyValuePair::resolve(const std::string_view input)
{
    value_pos = input.find(key);

    if (value_pos != input.npos) {
        value_pos += key_length;
        value = input.data() + value_pos;
        return true;
    }

    return false;
}
