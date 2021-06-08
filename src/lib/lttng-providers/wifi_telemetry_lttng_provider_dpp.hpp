
#undef TRACEPOINT_PROVIDER
#define TRACEPOINT_PROVIDER wifi_dpp

#undef TRACEPOINT_INCLUDE
#define TRACEPOINT_INCLUDE "./wifi_telemetry_lttng_provider_dpp.hpp"

#if !defined(__WIFI_TELEMETRY_LTTNG_PROVIDER_DPP_HPP__) || defined(TRACEPOINT_HEADER_MULTI_READ)
#define _TRACEPOINT_LTTNG_UST_TRACEF_PROVIDER_H

#define __WIFI_TELEMETRY_LTTNG_PROVIDER_DPP_HPP__

#include <lttng/tracepoint.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Event for a completed Device Provisioning Protocol (DPP) enrollee
 * exchange.
 * 
 * This event fires each time a DPP exchange completes with the host device
 * acting as the enrollee device role. The event has a state which describes
 * the furthest state of the exchange reached, which is a relative indication
 * of success or failure.
 * 
 * See the telemetry field descriptions below to determine which fields are
 * valid for specific state values.
 */
TRACEPOINT_EVENT(
    wifi_dpp,
    dpp_exchange_enrollee,
    TP_ARGS(
        const char *, interface,
        const char *, state,
        const char *, role,
        uint64_t, duration_milliseconds,
        size_t, chirp_frequencies_count,
        const uint32_t *, chirp_frequencies,
        const char *, failure_type,
        const char *, failure_details
    ),
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
         * @brief The last/furthest state of the dpp exchange reached [required].
         *
         * This field describes the furthest state of the dpp exchange reached.
         * The exchange was successful if the terminal state was entered,
         * indicated by the value "provisioned". The following are the possible
         * values:
         *
         *      - bootstrapping
         *          This is the initial state.
         *      - authenticated
         *          This state is entered once bootstrapping has completed.
         *      - provisioning
         *          This state is entered once the dpp peer has been successfully
         *          authenticated.
         *      - provisioned
         *          This state is entered once the enrollee has received a network
         *          configuration object. 
         *      - terminated
         *          This state is erntered if an error occurs during the exchange. In
         *          this case, the 'failure_type' and 'failure_details' fields provide
         *          more information about the cause of the failure.
         */
        ctf_string(state, state)

        /**
         * @brief DPP exchange role [required].
         * 
         * This field indicates the DPP role of the host, either initiator or
         * responder.
         *
         * The following strings are the possible values of this field:
         *  
         *      - initiator
         *      - responder 
         */
        ctf_string(role, role)

        /**
         * @brief Exchange duration (milliseconds) [required].
         * 
         * This fields describes the duration of the DPP exchange, in
         * milliseconds.
         *
         * The exchange is started when the first presence announcement (chirp)
         * is transmitted and completes when either a timeout occurs, an error
         * is encountered, or the enrollee is successfully provisioned.
         *
         * @example 2100
         */
        ctf_integer(uint64_t, duration_milliseconds, duration_milliseconds)

        /**
         * @brief Exchange presence announcement (chirp) frequencies [required].
         * 
         * This field describes all the frequencies where presence announcement
         * frames were sent. This is an additive, unique list that includes
         * every such frequency chirped, even if the last iteration of chirping
         * did not include a particular frequency.
         *
         * The chirp frequencies are encoded as an array of 32-bit unsigned
         * integers.
         */
        ctf_sequence(uint32_t, chirp_frequencies, chirp_frequencies, size_t, chirp_frequencies_count)

        /**
         * @brief The DPP failure type [conditional].
         * 
         * This field describes the category of failure that occurred if the
         * exchange failed. This field is only valid when the state field has
         * the value "terminated".
         *
         * The following strings are the possible values of this field:
         *
         *      - none
         *          This occurs when there was no error.
         *      - unspecified
         *          This occurs when the error is not known.
         *      - authentication-timeout
         *          This occurs when the DPP peer took too long to respond to the
         *          authentication request frame.
         */
        ctf_string(failure_type, failure_type)

        /**
         * @brief DPP failure details [conditional].
         * 
         * This field describes additional details regarding the cause of the
         * failure. This field is only valid when the state field has the value
         * "terminated". It is a free-form string.
         */
        ctf_string(failure_details, failure_details)))

/**
 * @brief Event for a complete dpp configurator exchange.
 */
TRACEPOINT_EVENT(
    wifi_dpp,
    dpp_exchange_configurator,
    TP_ARGS(
        const char *, interface,
        const char *, state,
        const char *, role,
        uint64_t, duration_milliseconds,
        const char *, bssid,
        uint32_t, frequency,
        const char *, failure_type,
        const char *, failure_details
    ),
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
         * @brief The last/furthest state of the dpp exchange reached [required].
         *
         * This field describes the furthest state of the dpp exchange reached.
         * The exchange was successful if the terminal state was entered,
         * indicated by the value "finished". The following are the possible
         * values:
         *
         *      - bootstrapping
         *          This is the initial state.
         *      - authenticated
         *          This state is entered once bootstrapping has completed.
         *      - finished
         *          This state is entered once the configurator has sent a network
         *          configuration object to the enrollee. 
         *      - terminated
         *          This state is erntered if an error occurs during the exchange. In
         *          this case, the 'failure_type' and 'failure_details' fields provide
         *          more information about the cause of the failure.
         */
        ctf_string(state, state)

        /**
         * @brief DPP exchange role [required].
         * 
         * This field indicates the DPP role of the host, either initiator or
         * responder.
         *
         * The following strings are the possible values of this field:
         *  
         *      - initiator
         *      - responder 
         */
        ctf_string(role, role)

        /**
         * @brief Exchange duration (milliseconds) [required].
         * 
         * This fields describes the duration of the DPP exchange, in
         * milliseconds.
         *
         * The exchange is started when the first presence announcement (chirp)
         * is received and completes when either a timeout occurs, an error
         * is encountered, or a network configuration object is sent to the
         * enrollee.
         *
         * @example 2100
         */
        ctf_integer(uint64_t, duration_milliseconds, duration_milliseconds)

        /**
         * @brief The Basic Service Set Identifier (BSSID) of the enrollee
         * [required].
         * 
         * This field describes the BSSID of the enrollee involved in the
         * exchange.
         * 
         * The data is formatted as a 48-bit sequence of hexadecimal pairs,
         * separated by a colon (:).
         * 
         * @example 70:66:55:09:b6:6f
         * @example 72:66:55:27:2c:1b
         */
        ctf_string(bssid, bssid)

        /**
         * @brief The radio frequency used for the exchange (MHz) [optional].
         * 
         * This field describes the radio frequency used for the exchange. This
         * field is always valid for all state field values except
         * 'bootstrapping', for which it is optionally valid. A value of zero
         * (0) indicates the field is invalid and should be ignored.
         *
         * @example 2412
         * @example 5180
         */
        ctf_integer(uint32_t, frequency, frequency)

        /**
         * @brief The DPP failure type [conditional].
         * 
         * This field describes the category of failure that occurred if the
         * exchange failed. This field is only valid when the state field has
         * the value "terminated".
         *
         * The following strings are the possible values of this field:
         *
         *      - none
         *          This occurs when there was no error.
         *      - unspecified
         *          This occurs when the error is not known.
         *      - authentication-timeout
         *          This occurs when the DPP peer took too long to respond to the
         *          authentication request frame.
         */
        ctf_string(failure_type, failure_type)

        /**
         * @brief DPP failure details [conditional].
         * 
         * This field describes additional details regarding the cause of the
         * failure. This field is only valid when the state field has the value
         * "terminated". It is a free-form string.
         */
        ctf_string(failure_details, failure_details)))

/**
 * @brief Event for Device Provisioning Protocol device role change.
 *
 * This event fires any time the active DPP device roles change on the host.
 * The event reports the currently active device roles (not the device role
 * that changed disposition).
 */
TRACEPOINT_EVENT(
    wifi_dpp,
    dpp_device_roles_changed,
    TP_ARGS(
        const char*, device_roles),
    TP_FIELDS(
        /**
         * @brief Active Device Provisioning Protocol (DPP) device roles
         * [required].
         * 
         * This field describes the currently active DPP device roles. There
         * are two possible values:
         * 
         *      - enrollee
         *      - configurator
         * 
         * This field is encoded as a single string using the comma (,) as a
         * delimeter for each active device role, with no additional spaces
         * separating the delimeter and subsequent role(s). The string may be
         * empty or contain any combination of the above values in any order.
         *
         * @example enrollee,configurator
         * @example enrollee
         * @example configurator
         * @example configurator,enrollee
         */
        ctf_string(device_roles, device_roles)))

#endif // !defined(__WIFI_TELEMETRY_LTTNG_PROVIDER_DPP_HPP__) || defined(TRACEPOINT_HEADER_MULTI_READ)

#include <lttng/tracepoint-event.h>
