
#include "wifi_telemetry_trace_dpp.hpp"
#include "wifi_telemetry_lttng_provider_dpp.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif //__clang__

#ifdef TRACE_USE_TRACELOGGING
// Provider UUID: 91adedb1-4f70-405f-9771-00415a5e375e
TRACELOGGING_DEFINE_PROVIDER(
    g_TraceloggingProviderWifiDpp,
    "Microsoft.Azure.Device.Wifi.Dpp",
    (0x91adedb1, 0x4f70, 0x405f, 0x97, 0x71, 0x00, 0x41, 0x5a, 0x5e, 0x37, 0x5e));
#endif // !TRACE_USE_TRACELOGGING

void
trace_dpp_exchange_enrollee(const char* interface, const char* state, const char* role, uint64_t duration_milliseconds, size_t chirp_frequencies_count, const uint32_t* chirp_frequencies, const char* failure_type, const char* failure_details)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingWrite(g_TraceloggingProviderWifiDpp,
        "DppExchangeEnrollee",
        TraceLoggingString(interface, "interface"),
        TraceLoggingString(state, "exchangeState"),
        TraceLoggingString(role, "dppDeviceRole"),
        TraceLoggingUInt64(duration_milliseconds, "durationMilliseconds"),
        TraceLoggingUInt32Array(chirp_frequencies, static_cast<uint16_t>(chirp_frequencies_count), "chirpFrequenciesMhz"),
        TraceLoggingString(failure_type, "failureType"),
        TraceLoggingString(failure_details, "failureDetails"));
#else  // TRACE_USE_TRACELOGGING
    tracepoint(wifi_dpp,
        dpp_exchange_enrollee,
        interface,
        role,
        state,
        duration_milliseconds,
        chirp_frequencies_count,
        chirp_frequencies,
        failure_type,
        failure_details);
#endif // !TRACE_USE_TRACELOGGING
}

void
trace_dpp_exchange_configurator(const char* interface, const char* state, const char* role, uint64_t duration_milliseconds, const char* bssid, uint32_t frequency, const char* failure_type, const char* failure_details)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingWrite(g_TraceloggingProviderWifiDpp,
        "DppExchangeConfigurator",
        TraceLoggingString(interface, "interface"),
        TraceLoggingString(state, "exchangeState"),
        TraceLoggingString(role, "dppDeviceRole"),
        TraceLoggingUInt64(duration_milliseconds, "durationMilliseconds"),
        TraceLoggingString(bssid, "bssid"),
        TraceLoggingUInt32(frequency, "frequencyMHz"),
        TraceLoggingString(failure_type, "failureType"),
        TraceLoggingString(failure_details, "failureDetails"));
#else  // TRACE_USE_TRACELOGGING
    tracepoint(wifi_dpp,
        dpp_exchange_configurator,
        interface,
        role,
        state,
        duration_milliseconds,
        bssid,
        frequency,
        failure_type,
        failure_details);
#endif // !TRACE_USE_TRACELOGGING
}

void
trace_dpp_device_roles_changed(const char* device_roles)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingWrite(g_TraceloggingProviderWifiDpp,
        "DppDeviceRolesChanged",
        TraceLoggingString(device_roles, "dppDeviceRolesActive"));
#else  // TRACE_USE_TRACELOGGING
    tracepoint(wifi_dpp,
        dpp_device_roles_changed,
        device_roles);
#endif // !TRACE_USE_TRACELOGGING
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif //__clang__
