
#ifndef __WPA_COMMAND_RESPONSE_HPP__
#define __WPA_COMMAND_RESPONSE_HPP__

#include <memory>
#include <string>
#include <string_view>

struct WpaCommandResponse
{
    virtual ~WpaCommandResponse() = default;
};

/**
 * @brief Used to send+receive unstructured commands+responses. 
 */
struct WpaCommandResponseGeneric : public WpaCommandResponse
{
    WpaCommandResponseGeneric(const std::string payload);

    std::string payload;
};

#endif //__WPA_COMMAND_RESPONSE_HPP__
