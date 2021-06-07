
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER wifi

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./wifi_telemetry_lttng_provider_wifi.hpp"

#if !defined(__WIFI_TELEMETRY_LTTNG_PROVIDER_WIFI_HPP__) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_LTTNG_UST_TRACEF_PROVIDER_H

#define __WIFI_TELEMETRY_LTTNG_PROVIDER_WIFI_HPP__

#include <lttng/tracepoint.h>
#include <stddef.h>
#include <stdint.h>

#include "wifi_device_info.hpp"

/**
 * @brief Event for wifi device/chipset information. 
 * 
 * This event fires periodically to report information about the wifi chipset
 * and driver being used for a particular interface. The event will fire each
 * start() is called on a WifiTelemetryMonitor instance. All telemetry events
 * generated from the monitor can then be associated with the device
 * information reported at the beginning of the session.
 */
TRACEPOINT_EVENT(
    wifi,
    device_info,
    TP_ARGS(
        const struct WifiDeviceInfo*, device),
    TP_FIELDS(
        /**
         * @brief Interface name [required].
         *
         * This fields describes the name of the wireless interface. It is
         * always included in the telemetry event. The interface may be virtual
         * or physical; no indication is provided in the event.
         *
         * The data is formatted as a free-form string, consisting of
         * printable ASCII characters.
         * 
         * @example wlan0
         * @example wlp4s0
         */
        ctf_string(interface, device->interface.c_str())

        /**
         * @brief Wi-Fi Basic Service Set Identifier (BSSID) [required].
         * 
         * This field describes the BSSID of the wireless device, which
         * uniquely identifies an 802.11-based wireless device.
         * 
         * The data is formatted as a 48-bit sequence of hexadecimal pairs,
         * separated by a colon (:).
         * 
         * @example 70:66:55:09:b6:6f
         * @example 72:66:55:27:2c:1b
         */
        ctf_string(bssid, device->bssid.c_str())

        /**
         * @brief Device subsystem [required].
         * 
         * This fields describes the kernel subsystem that is used to host the
         * device. This typically describes the device's bus.
         *
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.
         * 
         * @example pci
         * @example usb
         */
        ctf_string(subsystem, device->subsystem.c_str())

        /**
         * @brief Kernel driver (module) name [required].
         * 
         * This field describes the name of the kernel driver (module) used to
         * control the device.
         *
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.
         * 
         * @example ath10k
         * @example rtl88x2ce
         */
        ctf_string(kernel_driver, device->kernel_driver.c_str())

        /**
         * @brief Kernel driver (module) version [required].
         *
         * This fields describes the version string of the kernel driver
         * (module). Each module may define this string arbitrarily; it may
         * contain numerical version information.
         * 
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.
         * 
         * @example v5.9.0.3_36433.20200310_COEX20200103-1717
         */
        ctf_string(kernel_driver_version, device->kernel_driver_version.c_str())

        /**
         * @brief Device vendor name [optional].
         *
         * This fields describes the vendor of the device.
         *
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.
         * 
         * @example Intel Corporation
         * @example Realtek Semiconductor Co., Ltd.
         */
        ctf_string(vendor, device->vendor.value_or("").c_str())

        /**
         * @brief Device model name [optional].
         * 
         * This fields describes the device name or model of the device,
         * differentiating it from other devices manufactured by the vendor.
         *
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.
         *
         * @example Device c822
         */
        ctf_string(device, device->device.value_or("").c_str())

        /**
         * @brief Device subsystem vendor [optional].
         *
         * This field describes the name of the subsystem-specific vendor of
         * the device. This can be different from the device vendor field when
         * the bus portion of the device is implemented using different IHV
         * hardware components.
         *
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.
         * 
         * @example AzureWave
         */
        ctf_string(subsystem_vendor, device->subsystem_vendor.value_or("").c_str())

        /**
         * @brief Device subsystem model name [optional].
         * 
         * This field describes the name of the subsystem-specific vendor of
         * the device, differentiating it from other devices manufactured by
         * the subsystem vendor. This can be different from the device model
         * field when the bus potion of the device is implemented using
         * different IHV hardware components.
         * 
         * The data is formatted as a free-form string, consisting of printable
         * ASCII characters.

         * @example Device 3750
         */
        ctf_string(subsystem_device, device->subsystem_device.value_or("").c_str())))

#endif // !defined(__WIFI_TELEMETRY_LTTNG_PROVIDER_WIFI_HPP__) || defined(TRACEPOINT_HEADER_MULTI_READ)

#include <lttng/tracepoint-event.h>
