
#include "wifi_telemetry_instance_impl.hpp"

WifiTelemetrySourceInstanceImpl::WifiTelemetrySourceInstanceImpl(std::shared_ptr<WifiTelemetrySource> source_, const std::shared_ptr<WifiTelemetrySourceActivationArgs> args_) :
    WifiTelemetrySourceInstance(source_, args_)
{}

int
WifiTelemetrySourceInstanceImpl::activate(void)
{
    if (m_activated)
        return 0;

    int ret = source->activate(args);
    if (ret == 0)
        m_activated = true;

    return 0;
}

void
WifiTelemetrySourceInstanceImpl::deactivate(void)
{
    if (!m_activated)
        return;

    source->deactivate();
    m_activated = false;
}
