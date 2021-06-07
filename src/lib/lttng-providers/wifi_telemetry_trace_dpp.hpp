
#ifndef __WIFI_TELEMETRY_TRACE_DPP_HPP__
#define __WIFI_TELEMETRY_TRACE_DPP_HPP__

#include <cstdint>
#include <cstddef>

#ifdef TRACE_USE_TRACELOGGING
#include "wifi_telemetry_tracelogging.hpp"
#endif // !TRACE_USE_TRACELOGGING

#ifdef TRACE_USE_TRACELOGGING
TRACELOGGING_DECLARE_PROVIDER(g_TraceloggingProviderWifiDpp);
#endif // !TRACE_USE_TRACELOGGING

void
trace_dpp_exchange_enrollee(const char* interface, const char* state, const char* role, uint64_t duration_milliseconds, size_t chirp_frequencies_count, const uint32_t* chirp_frequencies, const char* failure_type, const char* failure_details);

void
trace_dpp_exchange_configurator(const char* interface, const char* state, const char* role, uint64_t duration_milliseconds, const char* bssid, uint32_t frequency, const char* failure_type, const char* failure_details);

void
trace_dpp_device_roles_changed(const char* device_roles);

#endif //__WIFI_TELEMETRY_TRACE_DPP_HPP__
