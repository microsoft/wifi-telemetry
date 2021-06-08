
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <string.h>
#include <string>

#include "wifi-telemetry/wifi_telemetry_monitor.hpp"
#include "wifi-telemetry/wifi_telemetry_source_wpa.hpp"
#include "wifi-telemetry/wifi_telemetry_source_ztpd.hpp"

static char*
triml(char* s)
{
    while (isspace(*s))
        s++;
    return s;
}

static WifiOperationalMode
parse_wifi_operational_mode(const char* operational_mode)
{
    if (strcmp(operational_mode, "ap") == 0 ||
        strcmp(operational_mode, "access-point") == 0) {
        return WifiOperationalMode::AccessPoint;
    } else if (strcmp(operational_mode, "sta") == 0 ||
        strcmp(operational_mode, "station") == 0) {
        return WifiOperationalMode::Station;
    }

    throw new std::invalid_argument("invalid operational mode string");
}

struct WifiTelemetrySourceArgs
{
    explicit WifiTelemetrySourceArgs(const std::string& name_) :
        name(name_)
    {}

    std::string name;
    std::optional<std::string> interface;
    std::optional<WifiOperationalMode> operational_mode;
};

int
main(int argc, char* argv[])
{
    WifiTelemetryMonitor monitor{};
    WifiTelemetrySourceArgs* source_active = nullptr;
    std::vector<WifiTelemetrySourceArgs> sources{};

    for (;;) {
        int opt = getopt(argc, argv, "i:o:s:");
        if (opt < 0)
            break;

        switch (opt) {
            case 'i':
                if (source_active)
                    source_active->interface = triml(optarg);
                break;
            case 'o':
                if (source_active)
                    source_active->operational_mode = parse_wifi_operational_mode(triml(optarg));
                break;
            case 's':
                source_active = &sources.emplace_back(triml(optarg));
                break;
            default:
                break;
        }
    }

    if (sources.size() == 0) {
        std::cerr << "no sources added" << std::endl;
        return -1;
    }

    for (const auto& source : sources) {
        // prepare interface configuration, if specified
        std::optional<WifiInterfaceConfiguration> interface;
        if (source.interface.has_value()) {
            interface.emplace(source.interface.value());
            if (source.operational_mode.has_value())
                interface->operational_mode = source.operational_mode.value();
        }

        // add the source
        if (source.name == "wpa")
            monitor.add_source<WifiTelemetrySourceWpa>(interface);
        else if (source.name == "ztp")
            monitor.add_source<WifiTelemetrySourceZtpd>(interface);
    }

    monitor.start();

    // Mask SIGTERM and SIGINT so they can be explicitly waited on from the main thread.
    int signal;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &mask, nullptr) < 0) {
        std::cerr << "failed to block terminate signals" << std::endl;
        return -1;
    }

    // Wait for the process to be signaled to exit.
    while (sigwait(&mask, &signal) != 0)
        ;

    // Received interrupt or terminate signal, so shutdown.
    monitor.stop();

    return 0;
}
