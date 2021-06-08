
#ifndef __WPA_COMMAND_KEY_VALUE_PAIR_HPP__
#define __WPA_COMMAND_KEY_VALUE_PAIR_HPP__

#include <cstddef>
#include <optional>
#include <string_view>

struct WpaCommandKeyValuePair
{
    WpaCommandKeyValuePair(const char* key_, bool is_required);

    const char*
    operator()(void) const;

    bool
    resolve(const std::string_view input);

    const char* key;
    const std::size_t key_length;
    std::optional<const char*> value;
    std::size_t value_pos;
    bool is_required;
};

enum
{
    WpaValueRequired = true,
    WpaValueOptional = false
};

#endif //__WPA_COMMAND_KEY_VALUE_PAIR_HPP__
