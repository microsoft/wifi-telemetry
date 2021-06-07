
#include "wifi_telemetry_trace.hpp"
#include "wifi_telemetry_trace_dpp.hpp"
#include "wifi_telemetry_trace_station.hpp"
#include "wifi_telemetry_trace_wifi.hpp"

#ifdef TRACE_USE_TRACELOGGING
#include "wifi_telemetry_tracelogging.hpp"
#endif // !TRACE_USE_TRACELOGGING

void
WifiTelemetryTraceProvider::RegisterAll(void)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingRegister(g_TraceloggingProviderWifi);
    TraceLoggingRegister(g_TraceloggingProviderWifiDpp);
    TraceLoggingRegister(g_TraceloggingProviderWifiStation);
#endif
}

void
WifiTelemetryTraceProvider::UnregisterAll(void)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingUnregister(g_TraceloggingProviderWifi);
    TraceLoggingUnregister(g_TraceloggingProviderWifiDpp);
    TraceLoggingUnregister(g_TraceloggingProviderWifiStation);
#endif
}
