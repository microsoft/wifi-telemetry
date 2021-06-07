
#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <unordered_set>

#include "wifi-telemetry/wifi_telemetry_monitor.hpp"
#include "wifi_device_info.hpp"
#include "wifi_telemetry_instance_impl.hpp"
#include "wifi_telemetry_trace.hpp"
#include "wifi_telemetry_trace_wifi.hpp"

WifiTelemetryMonitor::WifiTelemetryMonitor(void)
{
    WifiTelemetryTraceProvider::RegisterAll();
}

WifiTelemetryMonitor::~WifiTelemetryMonitor(void)
{
    WifiTelemetryTraceProvider::UnregisterAll();
}

void
WifiTelemetryMonitor::add_source_impl(const std::shared_ptr<WifiTelemetrySource> source, const std::shared_ptr<WifiTelemetrySourceActivationArgs> activation_args)
{
    auto instance = std::make_unique<WifiTelemetrySourceInstanceImpl>(source, activation_args);
    if (m_activated)
        instance->activate();

    m_sources.push_back(std::move(instance));
}

/**
 * @brief Removes a telemetry source from the monitor.
 * 
 * @param registration_token The registration token obtained from add_source().
 */
void
WifiTelemetryMonitor::remove_source(const std::shared_ptr<WifiTelemetrySource> source)
{
    const auto& result = std::find_if(std::begin(m_sources), std::end(m_sources), [&](const auto& instance) {
        return (instance->source == source);
    });

    if (result == std::end(m_sources))
        return;

    WifiTelemetrySourceInstanceImpl& instance = dynamic_cast<WifiTelemetrySourceInstanceImpl&>(**result);

    instance.deactivate();
    m_sources.erase(result);
}

std::size_t
WifiTelemetryMonitor::activate_sources(void)
{
    std::size_t num_activated = 0;
    std::unordered_set<std::string> interfaces;

    for (auto& instance : m_sources) {
        int ret = dynamic_cast<WifiTelemetrySourceInstanceImpl&>(*instance).activate();
        if (ret < 0) {
            std::cerr << "failed to activate telemetry source '" << instance->source->name() << "' (" << ret << ")" << std::endl;
            continue;
        }

        // Generate a telemetry event for each unique interface being monitored.
        if (instance->source->interface()) {
            const auto& [interface, inserted] = interfaces.insert(instance->source->interface()->name);
            if (inserted) {
                try {
                    WifiDeviceInfo device_info = WifiDeviceInfo::from_interface(*interface);
                    trace_wifi_device_info(&device_info);
                } catch (...) {
                    std::cout << "warning: failed to generate wifi device info telemetry event for " << *interface << std::endl;
                }
            }
        }

        num_activated++;
        std::cout << "activated telemetry source '" << instance->source->name() << "'" << std::endl;
    }

    return num_activated;
}

void
WifiTelemetryMonitor::deactivate_sources(void)
{
    for (auto& instance : m_sources) {
        dynamic_cast<WifiTelemetrySourceInstanceImpl&>(*instance).deactivate();
    }
}

void
WifiTelemetryMonitor::start(void)
{
    std::size_t num_activated_desired = m_sources.size();
    std::size_t num_activated_actual = activate_sources();
    std::cout << "telemetry monitor started with " << num_activated_actual << " of " << num_activated_desired
              << " telemetry sources" << std::endl;
    m_activated = true;
}

void
WifiTelemetryMonitor::stop(void)
{
    deactivate_sources();
    m_activated = false;
}
