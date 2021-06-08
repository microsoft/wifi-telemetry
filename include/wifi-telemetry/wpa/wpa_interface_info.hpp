
#ifndef __WPA_INTERFACE_INFO_HPP__
#define __WPA_INTERFACE_INFO_HPP__

#include <stdexcept>
#include <string>

#include "wifi-telemetry/wifi/wifi_common.hpp"

enum WpaType
{
    Hostapd,
    WpaSupplicant
};

/**
 * @brief Interface information common to all interfaces and modes.
 */
struct WpaInterfaceInfo
{
    std::string name;
    std::string address;

    static constexpr WpaType
    TypeFromOperationalMode(WifiOperationalMode mode)
    {
        switch (mode) {
            case WifiOperationalMode::Station:
                return WpaType::WpaSupplicant;
            case WifiOperationalMode::AccessPoint:
                return WpaType::Hostapd;
            default:
                throw new std::invalid_argument("no WpaType for operational mode");
        }
    }
};

#endif // __WPA_INTERFACE_INFO_HPP__
