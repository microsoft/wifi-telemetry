
#ifndef __WIFI_TELEMETRY_TRACE_HPP__
#define __WIFI_TELEMETRY_TRACE_HPP__

struct WifiTelemetryTraceProvider
{
    /**
     * @brief Registers all Tracelogging-based providers with LLTNG. This must
     * be called prior to generating any tracelogging-based telemetry events.
     */
    static void
    RegisterAll(void);

    /**
     * @brief Unregisters all Tracelogging-based providers from LTTNG.
     */
    static void
    UnregisterAll(void);
};

#endif //__WIFI_TELEMETRY_TRACEPROVIDERS_HPP__
