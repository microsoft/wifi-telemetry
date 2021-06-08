
#ifndef __WIFI_TELEMETRY_TRACEPROVIDER_WIFI_HPP__
#define __WIFI_TELEMETRY_TRACEPROVIDER_WIFI_HPP__

#include "wifi_device_info.hpp"
#ifdef TRACE_USE_TRACELOGGING
#include "wifi_telemetry_tracelogging.hpp"
#endif // !TRACE_USE_TRACELOGGING

#ifdef TRACE_USE_TRACELOGGING
TRACELOGGING_DECLARE_PROVIDER(g_TraceloggingProviderWifi);
#endif // !TRACE_USE_TRACELOGGING

void
trace_wifi_device_info(const struct WifiDeviceInfo* info);

#endif //__WIFI_TELEMETRY_TRACEPROVIDER_WIFI_HPP__
