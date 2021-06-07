
#ifndef __WIFI_TELEMETRY_TRACE_STATION_HPP__
#define __WIFI_TELEMETRY_TRACE_STATION_HPP__

#ifdef TRACE_USE_TRACELOGGING
#include "wifi_telemetry_tracelogging.hpp"
#endif // !TRACE_USE_TRACELOGGING
#include "wifi-telemetry/wpa/wifi_telemetry_input_station_connection_attempt.hpp"

#ifdef TRACE_USE_TRACELOGGING
TRACELOGGING_DECLARE_PROVIDER(g_TraceloggingProviderWifiStation);
#endif // !TRACE_USE_TRACELOGGING

void 
trace_station_connection_attempt(const char *interface, const WifiTelemetryInputStationConnectionAttempt *data);

void
trace_station_connection_drop(const char *interface, const char *bssid, uint16_t reason_code, int locally_generated);

#endif //__WIFI_TELEMETRY_TRACE_STATION_HPP__
