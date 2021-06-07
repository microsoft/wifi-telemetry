
#ifndef __WIFI_TELEMETRY_SOURCE_INSTANCE_IMPL_HPP__
#define __WIFI_TELEMETRY_SOURCE_INSTANCE_IMPL_HPP__

#include "wifi-telemetry/wifi_telemetry_source_instance.hpp"

struct WifiTelemetrySourceInstanceImpl :
    public WifiTelemetrySourceInstance
{
    WifiTelemetrySourceInstanceImpl(std::shared_ptr<WifiTelemetrySource> source, const std::shared_ptr<WifiTelemetrySourceActivationArgs> args);

    int
    activate(void);

    void
    deactivate(void);

private:
    bool m_activated = false;
};

#endif // __WIFI_TELEMETRY_SOURCE_INSTANCE_IMPL_HPP__
