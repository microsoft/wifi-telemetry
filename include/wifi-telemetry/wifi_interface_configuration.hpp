
#ifndef __WIFI_INTERFACE_CONFIGURATION_HPP__
#define __WIFI_INTERFACE_CONFIGURATION_HPP__

#include <string>

#include "wifi-telemetry/wifi/wifi_common.hpp"

struct WifiInterfaceConfiguration
{
    WifiInterfaceConfiguration(const std::string& name_, const WifiOperationalMode operational_mode_) :
        name(name_),
        operational_mode(operational_mode_)
    {}

    WifiInterfaceConfiguration(const std::string& name_) :
        WifiInterfaceConfiguration(name_, WifiOperationalMode::Station)
    {}

    std::string name;
    WifiOperationalMode operational_mode;
};

#endif //__WIFI_INTERFACE_CONFIGURATION_HPP__
