
#ifndef __WPA_EVENT_DPP_CONF_RECEIVED_HPP__
#define __WPA_EVENT_DPP_CONF_RECEIVED_HPP__

#include "wifi-telemetry/wpa/wpa_event.hpp"

struct WpaEventDppConfigurationReceived : public WpaEvent
{
    WpaEventDppConfigurationReceived(void);
};

#endif // __WPA_EVENT_DPP_CONF_RECEIVED_HPP__
