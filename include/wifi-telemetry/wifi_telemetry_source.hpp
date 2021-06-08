
#ifndef __WIFI_TELEMETRY_SOURCE_HPP__
#define __WIFI_TELEMETRY_SOURCE_HPP__

#include <memory>
#include <optional>
#include <string>
#include <string_view>

#include "wifi-telemetry/wifi/wifi_common.hpp"
#include "wifi-telemetry/wifi_interface_configuration.hpp"

struct WifiTelemetrySourceActivationArgs
{
    ~WifiTelemetrySourceActivationArgs(void) = default;
};

class WifiTelemetrySource
{
protected:
    WifiTelemetrySource(const std::optional<WifiInterfaceConfiguration>& interface);
    WifiTelemetrySource(const std::optional<WifiInterfaceConfiguration>& interface, const std::string& name);

public:
    virtual ~WifiTelemetrySource(void);

    virtual int
    activate(const std::shared_ptr<WifiTelemetrySourceActivationArgs> args);

    virtual void
    deactivate(void);

    virtual const std::string_view
    name() const;

    const std::optional<WifiInterfaceConfiguration>&
    interface(void) const;

protected:
    std::string m_name = "unnamed";
    const std::optional<WifiInterfaceConfiguration> m_interface;
};

#endif //__WIFI_TELEMETRY_SOURCE_HPP__
