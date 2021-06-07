
#ifndef __WIFI_TELEMETRY_SOURCE_ZTPD_HPP__
#define __WIFI_TELEMETRY_SOURCE_ZTPD_HPP__

#include <memory>
#include <optional>

#include <sdbus-c++/sdbus-c++.h>

#include "wifi-telemetry/wifi_telemetry_source.hpp"

class __attribute__((visibility("default"))) WifiTelemetrySourceZtpd :
    public WifiTelemetrySource,
    public sdbus::ProxyInterfaces<sdbus::Properties_proxy>
{
public:
    WifiTelemetrySourceZtpd(const std::optional<WifiInterfaceConfiguration>& interface_config);
    ~WifiTelemetrySourceZtpd(void);

    int
    activate(const std::shared_ptr<WifiTelemetrySourceActivationArgs> args) override;

    void
    deactivate(void) override;

private:
    // sdbus::Properties_proxy
    void
    onPropertiesChanged(const std::string& interfaceName,
        const std::map<std::string, sdbus::Variant>& changedProperties,
        const std::vector<std::string>& invalidatedProperties) override;
};

#endif //__WIFI_TELEMETRY_SOURCE_ZTPD_HPP__
