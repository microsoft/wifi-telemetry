
#include <numeric>

#include "wifi-telemetry/wifi_telemetry_source_ztpd.hpp"
#include "wifi_telemetry_trace_dpp.hpp"

#define ZTP_DBUS_INTERFACE "com.microsoft.ztp1"
#define ZTP_DBUS_PATH "/com/microsoft/ztp1"

WifiTelemetrySourceZtpd::WifiTelemetrySourceZtpd(const std::optional<WifiInterfaceConfiguration>& interface) :
    WifiTelemetrySource(interface, "ztpd"),
    ProxyInterfaces(ZTP_DBUS_INTERFACE, ZTP_DBUS_PATH)
{}

WifiTelemetrySourceZtpd::~WifiTelemetrySourceZtpd(void)
{}

int
WifiTelemetrySourceZtpd::activate(const std::shared_ptr<WifiTelemetrySourceActivationArgs> /* args */)
{
    registerProxy();
    return 0;
}

void
WifiTelemetrySourceZtpd::deactivate(void)
{
    unregisterProxy();
}

void
WifiTelemetrySourceZtpd::onPropertiesChanged(const std::string& interfaceName, const std::map<std::string, sdbus::Variant>& changedProperties, const std::vector<std::string>& /* invalidatedProperties */)
{
    if (interfaceName != ZTP_DBUS_INTERFACE)
        return;

    const auto roles_it = changedProperties.find("Roles");
    if (roles_it == changedProperties.end())
        return;

    std::string roles{};
    const auto roles_changed = roles_it->second.get<std::vector<std::string>>();

    if (!roles_changed.empty()) {
        roles = std::accumulate(
            /* start */ std::next(std::begin(roles_changed)),
            /*   end */ std::end(roles_changed),
            /*  init */ roles_changed.front(),
            /*    op */ [](std::string current, const std::string& next)
                        {
                            return std::move(current) + ',' + next;
                        });
    }

    trace_dpp_device_roles_changed(roles.c_str());
}
