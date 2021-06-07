
#include "wifi-telemetry/wifi_telemetry_source.hpp"

WifiTelemetrySource::WifiTelemetrySource(const std::optional<WifiInterfaceConfiguration>& interface) :
    m_interface(interface)
{}

WifiTelemetrySource::WifiTelemetrySource(const std::optional<WifiInterfaceConfiguration>& interface, const std::string& name) :
    m_name(name),
    m_interface(interface)
{}

WifiTelemetrySource::~WifiTelemetrySource(void)
{}

int
WifiTelemetrySource::activate(const std::shared_ptr<WifiTelemetrySourceActivationArgs> /* args */)
{
    return 0;
}

void
WifiTelemetrySource::deactivate(void)
{}

const std::string_view
WifiTelemetrySource::name(void) const
{
    return m_name;
}

const std::optional<WifiInterfaceConfiguration>&
WifiTelemetrySource::interface(void) const
{
    return m_interface;
}
