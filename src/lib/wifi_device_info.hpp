
#ifndef __WIFI_DEVICE_INFO_HPP__
#define __WIFI_DEVICE_INFO_HPP__

#include <optional>
#include <string>

struct WifiDeviceInfo
{
    WifiDeviceInfo(
        const std::string& interface,
        const std::string& bssid,
        const std::string& subsystem,
        const std::string& kernel_driver,
        const std::string& kernel_driver_version);

    const std::string interface;
    const std::string bssid;
    const std::string subsystem;
    const std::string kernel_driver;
    const std::string kernel_driver_version;

    std::optional<std::string> vendor;
    std::optional<std::string> device;
    std::optional<std::string> subsystem_vendor;
    std::optional<std::string> subsystem_device;

    static WifiDeviceInfo
    from_interface(const std::string& interface);
};

#endif // __WIFI_DEVICE_INFO_HPP__
