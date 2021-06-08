
#include "wifi_telemetry_trace_station.hpp"
#include "wifi_telemetry_lttng_provider_station.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif //__clang__

#ifdef TRACE_USE_TRACELOGGING
// Provider UUID: 00bb2910-064f-41b3-8ead-ad6d608a10d0
TRACELOGGING_DEFINE_PROVIDER(
    g_TraceloggingProviderWifiStation,
    "Microsoft.Azure.Device.Wifi.Station",
    (0x00bb2910, 0x064f, 0x41b3, 0x8e, 0xad, 0xad, 0x6d, 0x60, 0x8a, 0x10, 0xd0));
#endif // !TRACE_USE_TRACELOGGING

void
trace_station_connection_attempt(const char* interface, const WifiTelemetryInputStationConnectionAttempt* data)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingWrite(g_TraceloggingProviderWifiStation,
        "ConnectionAttempt",
        TraceLoggingString(interface, "interface"),
        TraceLoggingUInt32(data->result(), "result"),
        TraceLoggingString(data->bssid(), "bssid"),
        TraceLoggingString(data->ssid(), "ssid"),
        TraceLoggingInt32(data->rssi(), "rssi"),
        TraceLoggingInt32(data->noise(), "noiseDb"),
        TraceLoggingInt32(data->link_speed(), "linkSpeedMbps"),
        TraceLoggingUInt32(data->frequency(), "frequencyMHz"),
        TraceLoggingUInt32(data->wifi_generation(), "wifiGeneration"),
        TraceLoggingString(data->key_mgmt(), "keyManagement"),
        TraceLoggingString(data->pairwise_cipher(), "pairwiseCipher"),
        TraceLoggingString(data->group_cipher(), "groupCipher"),
        TraceLoggingUInt16(data->status_code(), "statusCode"),
        TraceLoggingString(data->authentication_type(), "authenticationType"));
#else  // TRACE_USE_TRACELOGGING
    tracepoint(wifi_station,
        connection_attempt,
        interface,
        data);
#endif // !TRACE_USE_TRACELOGGING
}

void
trace_station_connection_drop(const char* interface, const char* bssid, uint16_t reason_code, int locally_generated)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingWrite(g_TraceloggingProviderWifiStation,
        "ConnectionDrop",
        TraceLoggingString(interface, "interface"),
        TraceLoggingString(bssid, "bssid"),
        TraceLoggingUInt16(reason_code, "reasonCode"),
        TraceLoggingBoolean(!!locally_generated, "locallyGenerated"));
#else  // TRACE_USE_TRACELOGGING
    tracepoint(wifi_station,
        connection_drop,
        interface,
        bssid,
        reason_code,
        locally_generated);
#endif // !TRACE_USE_TRACELOGGING
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif //__clang__
