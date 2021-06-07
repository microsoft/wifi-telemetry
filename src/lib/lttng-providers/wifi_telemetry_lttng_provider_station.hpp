
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER wifi_station

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./wifi_telemetry_lttng_provider_station.hpp"

#if !defined(__WIFI_TELEMETRY_LTTNG_PROVIDER_STATION_HPP__) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_LTTNG_UST_TRACEF_PROVIDER_H

#define __WIFI_TELEMETRY_LTTNG_PROVIDER_STATION_HPP__

#include <lttng/tracepoint.h>
#include <stddef.h>
#include <stdint.h>

#include "wifi-telemetry/wpa/wifi_telemetry_input_station_connection_attempt.hpp"

/**
 * @brief Event for a connection attempt from a wifi station (client) to an
 * access point or other peer.
 *
 * This event fires upon completion of all connection attempts. The attempt has
 * a result which describes whether it was successful, and result-specific
 * fields that further describe details of the result; connection state
 * information when succesful, and failure details otherwise.
 * 
 * See the telemetry field descriptions below to determine which fields are
 * valid for specific result values.
 */
TRACEPOINT_EVENT(
    wifi_station,
    connection_attempt,
    TP_ARGS(
        const char*, interface, 
        const WifiTelemetryInputStationConnectionAttempt*, data),
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
        ctf_string(interface, interface)

        /**
         * @brief The result of the connection attempt [required].
         * 
         * This field describes the coarse result of the connection attempt.
         *
         * The data is formatted as an unsigned, 32-bit integer. @see
         * WifiConnectionAttemptResult for a complete list of all possible
         * values this field can take.
         *
         * result values and sub-fields:
         *
         * - WifiConnectionAttemptResult::Succeeded (value=0)
         *      This result occurs when the connection attempt was successful and a
         *      valid connection was made. In this case, the following telemetry
         *      fields will further describe the state of the connection:
         * 
         *          - bssid
         *          - ssid
         *          - rssi
         *          - noise
         *          - link_speed
         *          - frequency
         *          - wifi_generation
         *          - key_mgmt
         *          - group_cipher
         *
         * - WifiConnectionAttemptResult::Unspecified (value=1)
         *      This result occurs when the connection attempt failed and no
         *      specific reason for the failure could be determined. No additional
         *      fields are valid for this result value.
         *
         * - WifiConnectionAttemptResult::AuthenticationRejected (value=2)
         *      This result occurs when the connection attempt failed due to
         *      authentication being rejected by the peer. In this case, the
         *      following telemetry fields in this event will further describe the
         *      reason for failure:
         *
         *          - bssid
         *          - status_code
         *          - authentication_type
         *
         *      The status code field describes the specific reason for failure. 
         *      See the comments in status code for more details. The
         *      autnentication type field describes the type of authentication
         *      attempted.
         *      
         * - WifiConnectionAttemptResult::AssociationRejected (value=3)
         *      This result occurs when the connection attempt failed due to
         *      association being rejected. In this case, the following telemetry
         *      fields in this event will further describe the reason for failure:
         *
         *          - bssid
         *          - status_code
         *
         *      The status code field describes the specific reason for failure. See
         *      the comments in status code for more details.
         *
         * - WifiConnectionAttemptResult::NetworkNotFound:
         *      This result occurs whenm the connection attempt failed due to the
         *      selected network not being found. This can occur when either the
         *      target network is not in radio range, or when the target network was
         *      found but does not match the characteristics of its profile. For
         *      example, this can occur when the network profile specifies the
         *      target network uses one type of authentication, but the actual
         *      network uses a different type. In this case, no additional telemetry
         *      fields are valid.
         */
        ctf_integer(uint32_t, result, data->result())

        /**
         * @brief The Basic Service Set Identifier (BSSID) of the connection
         * attempt peer [conditional].
         * 
         * This field describes the BSSID of the wireless device peer involved
         * in the connection attempt.
         * 
         * The data is formatted as a 48-bit sequence of hexadecimal pairs,
         * separated by a colon (:).
         * 
         * @example 70:66:55:09:b6:6f
         * @example 72:66:55:27:2c:1b
         */
        ctf_string(bssid, data->bssid())

        /**
         * @brief The Service Set Identifier (SSID) of the peer [conditional].
         *
         * This field describes the SSID of the wireless device peer involved
         * in the connection attempt.
         *
         * @example My Home Network
         * @example Microsoft-Guest
         */
        ctf_string(ssid, data->ssid())

        /**
         * @brief Connection Received Signal Strength Indicator (RSSI) [conditional].
         *
         * This field describes the RSSI of the connection at the time
         * the connection was established. This is technically a unit-less
         * value, however, is typically reported in dBm. RSSI units and values
         * are vendor specific and so should not be compared across vendors and
         * in some cases, even different devices.
         *
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         * 
         * @example -70
         * @example 10
         */
        ctf_integer(int32_t, rssi, data->rssi())

        /**
         * @brief Connection noise level (dB) [conditional].
         *
         * This fields describes the noise level of the connection at the time
         * the connection was established. The unit is decibels (dB).
         *
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_integer(int32_t, noise, data->noise())

        /**
         * @brief Connection link speed (Mbps) [conditional].
         *
         * This fields describes the link speed of the connection at the time
         * the connection was established. The unit is Mbps.
         * 
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_integer(int32_t, link_speed, data->link_speed())

        /**
         * @brief Connection frequency (MHz) [conditional].
         *
         * This field describes the frequency of the connection at the time the
         * connection was established. The unit is MHz.
         * 
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_integer(uint32_t, frequency, data->frequency())

        /**
         * @brief Connection Wi-Fi generation [conditional].
         * 
         * This field describes the numerical Wi-Fi generation of the
         * connection. This corresponds to specific IEEE Wi-Fi standards,
         * starting with 802.11n (4), 802.11ac (5), 802.11ax (6).
         *
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_integer(uint32_t, wifi_generation, data->wifi_generation())

        /**
         * @brief Connection key management [conditional].
         * 
         * This field describes the key management method used by the
         * connection. The following strings are some of the possible values:
         * 
		 *      - NONE
		 *      - WPA-NONE
		 *      - IEEE 802.1X (no WPA)
		 *      - FT-EAP
		 *      - FT-EAP-SHA384
		 *      - FT-PSK
		 *      - WPA2-EAP-SHA256
		 *      - WPA2-PSK-SHA256
		 *      - WPS
		 *      - SAE
		 *      - FT-SAE
		 *      - OSEN
		 *      - WPA2-EAP-SUITE-B
		 *      - WPA2-EAP-SUITE-B-192
		 *      - FILS-SHA256
		 *      - FILS-SHA384
		 *      - FT-FILS-SHA256
		 *      - FT-FILS-SHA384
		 *      - OWE
		 *      - DPP
		 *      - UNKNOWN 
         *
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_string(key_mgmt, data->key_mgmt())

        /**
         * @brief Connection pairwise cipher [conditional].
         *
         * This fields describes the connection pairwise cipher. The pairwise
         * cipher is used for unicast (point-to-point) traffic.
         * 
         * The following strings are some of the possible values:
         *      - NONE
		 *      - WEP-40
		 *      - WEP-104
		 *      - TKIP
		 *      - CCMP
		 *      - CCMP+TKIP
		 *      - GCMP
		 *      - GCMP-256
		 *      - CCMP-256
		 *      - BIP
		 *      - BIP-GMAC-128
		 *      - BIP-GMAC-256
		 *      - BIP-CMAC-256
		 *      - GTK_NOT_USED
		 *      - UNKNOWN
         *
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_string(pairwise_cipher, data->pairwise_cipher())

        /**
         * @brief Connection group cipher [conditional].
         *
         * This fields describes the connection group cipher. The group cipher
         * is used for multicast traffic.
         * 
         * The possible values are the same as pairwise_cipher.
         *
         * This field is only valid when result == 0 (Succeeded) and is
         * otherwise undefined and must be discarded.
         */
        ctf_string(group_cipher, data->group_cipher())

        /**
         * @brief The Wi-Fi status code of the connection attempt [conditional].
         *
         * This fields describes the Wi-Status code of the connection attempt.
         * The values are defined in IEEE Std 802.11-2016, Section 9.4.1.9,
         * Table 9-46. @see WifiStatusCode for a more detailed description.
         * 
         * This field is only valid when:
         *      result == 2 (AuthenticationRejected) OR
         *      result == 3 (AssociationRejected)
         * and is otherwise undefined and must be discarded
         */
        ctf_integer(uint16_t, status_code, data->status_code())

        /**
         * @brief The connection authentication type [conditional].
         * 
         * This field describes the general authentication type of the
         * connnection attempt.
         * 
         * The following strings are some of the possible values:
         * 
         *      - unknown
         *      - open
         *      - wpa-psk
         *      - shared
         *      - wpa
         *      - wpa2
         *      - wpa2-psk
         * 
         * This field is only valid when result == 2
         * (AuthenticationRejected) and is otherwise undefined and must be
         * discarded.
         */
        ctf_string(authentication_type, data->authentication_type())))

/**
 * @brief Event for a connection drop from an access point or other wireless peer.
 *
 * This event fires any time an established wireless connection is severred or
 * dropped, regardless of whether it was a healthy/expected disconnection or an
 * unhealthy/unexpected one.
 */
TRACEPOINT_EVENT(
    wifi_station,
    connection_drop,
    TP_ARGS(
        const char*, interface, 
        const char*, bssid, 
        uint16_t, reason_code, 
        int, locally_generated),
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
        ctf_string(interface, interface)

        /**
         * @brief The Basic Service Set Identifier (BSSID) of the connection
         * attempt peer [required].
         * 
         * This field describes the BSSID of the wireless device peer that was
         * disconnected.
         * 
         * The data is formatted as a 48-bit sequence of hexadecimal pairs,
         * separated by a colon (:).
         * 
         * @example 70:66:55:09:b6:6f
         * @example 72:66:55:27:2c:1b
         */
        ctf_string(bssid, bssid)

        /**
         * @brief The reason code for disassociation [required].
         * 
         * This field describes the reason for disassociation (connection
         * drop). The values are defined in IEEE Std 802.11-2016, Section
         * 9.4.1.4, Table 9-45. @see WifiDeauthenticationReasonCode for a more
         * detailed description.
         */
        ctf_integer(uint16_t, reason_code, reason_code)

        /**
         * @brief Local generation/request indicator [required].
         *
         * This field describes whether the connection drop originated from the
         * client (locally_generated != 0) or originated from the peer
         * (locally_generated == 0).
         */
        ctf_integer(int, locally_generated, locally_generated)))

#endif // !defined(__WIFI_TELEMETRY_LTTNG_PROVIDER_STATION_HPP__) || defined(TRACEPOINT_HEADER_MULTI_READ

#include <lttng/tracepoint-event.h>
