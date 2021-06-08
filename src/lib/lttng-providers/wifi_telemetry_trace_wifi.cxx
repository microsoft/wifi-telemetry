
#include "wifi_telemetry_trace_wifi.hpp"
#include "wifi_telemetry_lttng_provider_wifi.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"
#pragma clang diagnostic ignored "-Wgnu-statement-expression"
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif //__clang__

#ifdef TRACE_USE_TRACELOGGING
// Provider UUID: d4238472-0bb9-4b64-af1e-14f3f7a5411b
TRACELOGGING_DEFINE_PROVIDER(
    g_TraceloggingProviderWifi,
    "Microsoft.Azure.Device.Wifi",
    (0xd4238472, 0x0bb9, 0x4b64, 0xaf, 0x1e, 0x14, 0xf3, 0xf7, 0xa5, 0x41, 0x1b));
#endif // !TRACE_USE_TRACELOGGING

void
trace_wifi_device_info(const struct WifiDeviceInfo* device)
{
#ifdef TRACE_USE_TRACELOGGING
    TraceLoggingWrite(g_TraceloggingProviderWifi,
        "DeviceInfo",
        TraceLoggingString(device->interface.c_str(), "interface"),
        TraceLoggingString(device->bssid.c_str(), "bssid"),
        TraceLoggingString(device->subsystem.c_str(), "subsystem"),
        TraceLoggingString(device->kernel_driver.c_str(), "kernelDriver"),
        TraceLoggingString(device->kernel_driver_version.c_str(), "kernelDriverVersion"),
        TraceLoggingString(device->vendor.value_or("").c_str(), "vendor"),
        TraceLoggingString(device->device.value_or("").c_str(), "model"),
        TraceLoggingString(device->subsystem_vendor.value_or("").c_str(), "subsystemVendor"),
        TraceLoggingString(device->subsystem_device.value_or("").c_str(), "subsystemModel"));
#else  // TRACE_USE_TRACELOGGING
    tracepoint(wifi,
        device_info,
        device);
#endif // !TRACE_USE_TRACELOGGING
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif //__clang__
