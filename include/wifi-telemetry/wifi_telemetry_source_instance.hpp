
#ifndef __WIFI_TELEMETRY_SOURCE_INSTANCE_HPP__
#define __WIFI_TELEMETRY_SOURCE_INSTANCE_HPP__

#include <memory>

#include "wifi-telemetry/wifi_telemetry_source.hpp"

struct WifiTelemetrySourceInstance
{
public:
    WifiTelemetrySourceInstance(std::shared_ptr<WifiTelemetrySource> source_, const std::shared_ptr<WifiTelemetrySourceActivationArgs> args_) :
        source(std::move(source_)),
        args(args_)
    {}

    virtual ~WifiTelemetrySourceInstance(void) = default;

    std::shared_ptr<WifiTelemetrySource> source;
    std::shared_ptr<WifiTelemetrySourceActivationArgs> args;
};

#endif // __WIFI_TELEMETRY_SOURCE_INSTANCE_HPP__
