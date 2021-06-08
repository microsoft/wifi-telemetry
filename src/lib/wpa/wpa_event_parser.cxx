
#include <cinttypes>
#include <functional>
#include <iostream>
#include <wpa_ctrl.h>

#include "wifi-telemetry/wpa/wpa_events.hpp"
#include "wpa_event_parser.hpp"

static std::shared_ptr<WpaEvent>
parse_event_connected(const std::string_view message)
{
    wifi_80211_mac bssid;

    int ret = std::sscanf(message.data(), "- Connection to " WIFI_80211_MAC_FMT, STR_2_WIFI80211_MAC(bssid));
    if (ret < 0) {
        std::cerr << "failed to parse wpa connected event message (invalid format)" << std::endl;
        return nullptr;
    }

    return std::make_shared<WpaEventConnected>(bssid);
}

static std::shared_ptr<WpaEvent>
parse_event_disconnected(const std::string_view message)
{
    static const char *fmt_without_locally_generated = "bssid=" WIFI_80211_MAC_FMT " reason=%d";
    static const char *fmt_with_locally_generated = "bssid=" WIFI_80211_MAC_FMT " reason=%d locally_generated=%1d";

    int ret;
    int reason_code;
    int locally_generated;
    wifi_80211_mac bssid;

    ret = std::sscanf(message.data(), fmt_with_locally_generated, STR_2_WIFI80211_MAC(bssid), &reason_code, &locally_generated); 
    if (ret != 8) {
        locally_generated = 0;
        ret = std::sscanf(message.data(), fmt_without_locally_generated, STR_2_WIFI80211_MAC(bssid), &reason_code);
        if (ret != 7) {
            std::cerr << "failed to parse wpa disconnected event message (invalid format)" << std::endl;
            return nullptr;
        }
    }

    // TODO: below cast to WifiDeauthenticationReasonCode should really be checked for validity.
    return std::make_shared<WpaEventDisconnected>(bssid, static_cast<WifiDeauthenticationReasonCode>(reason_code), !!locally_generated);
}

static std::shared_ptr<WpaEvent>
parse_event_association_rejected(const std::string_view message)
{
    int ret;
    uint32_t status_code;
    wifi_80211_mac bssid;

    ret = std::sscanf(message.data(), "bssid=" WIFI_80211_MAC_FMT " status_code=%u", STR_2_WIFI80211_MAC(bssid), &status_code);
    if (ret != 7) {
        ret = std::sscanf(message.data(), "status_code=%u", &status_code);
        if (ret != 1) {
            std::cerr << "failed to parse wpa association rejected event (invalid format)" << std::endl;
            return nullptr;
        }

        return std::make_shared<WpaEventAssociationRejected>(WifiStatusCode(status_code));
    }

    return std::make_shared<WpaEventAssociationRejected>(WifiStatusCode(status_code), bssid);
}

static std::shared_ptr<WpaEvent>
parse_event_authentication_rejected(const std::string_view message)
{
    int ret;
    uint32_t auth_type = 0;
    uint32_t status_code = 0;
    wifi_80211_mac bssid = wifi_80211_any_address;

    ret = std::sscanf(message.data(), WIFI_80211_MAC_FMT " auth_type=%u status_code=%u", STR_2_WIFI80211_MAC(bssid), &status_code, &auth_type);
    if (ret != 8) {
        std::cerr << "failed to parse wpa authentication rejected event (invalid format)" << std::endl;
        return nullptr;
    }

    return std::make_shared<WpaEventAuthenticationRejected>(bssid, WifiStatusCode(status_code), WifiAuthenticationType(auth_type));
}

static std::shared_ptr<WpaEvent>
parse_event_network_not_found(const std::string_view /* message */)
{
    return std::make_shared<WpaEventNetworkNotFound>();
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_auth_success(const std::string_view message)
{
    int initiator;
    int ret = std::sscanf(message.data(), "init=%d", &initiator);
    if (ret < 0) {
        std::cerr << "failed to parse wpa dpp auth success event message (invalid format)" << std::endl;
        return nullptr;
    }

    return std::make_shared<WpaEventDppAuthenticationSucceeded>(initiator);
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_conf_received(const std::string_view /* message */)
{
    return std::make_shared<WpaEventDppConfigurationReceived>();
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_conf_sent(const std::string_view /* message */)
{
    return std::make_shared<WpaEventDppConfigurationSent>();
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_fail(const std::string_view message)
{
    const std::string details = message.data();
    return std::make_shared<WpaEventDppFailure>(details);
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_auth_init_fail(const std::string_view /* message */)
{
    return std::make_shared<WpaEventDppAuthenticationInitFailure>();
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_chirp_rx(const std::string_view message)
{
    int id;
    unsigned frequency;
    wifi_80211_mac bssid;

    int ret = std::sscanf(message.data(), "id=%d src=" WIFI_80211_MAC_FMT " freq=%u hash=%*s",
        &id, STR_2_WIFI80211_MAC(bssid), &frequency);
    if (ret < 0) {
        std::cerr << "failed to parse wpa dpp chirp rx event message (invalid format)" << std::endl;
        return nullptr;
    }

    return std::make_shared<WpaEventDppChirpReceived>(id, bssid, frequency);
}

static std::shared_ptr<WpaEvent>
parse_event_dpp_tx_status(const std::string_view message)
{
    char status[128];
    unsigned frequency;
    wifi_80211_mac bssid;

    int ret = std::sscanf(message.data(), "dst=" WIFI_80211_MAC_FMT " freq=%u result=%127s",
        STR_2_WIFI80211_MAC(bssid), &frequency, status);
    if (ret < 0) {
        std::cerr << "failed to parse wpa dpp tx status event message (invalid format)" << std::endl;
        return nullptr;
    }

    return std::make_shared<WpaEventDppFrameTransmitStatus>(bssid, frequency, status);
}

typedef std::function<std::shared_ptr<WpaEvent>(const std::string_view message)> wpa_event_parser;

struct WpaEventMessageParser 
{
    const std::string_view prefix;
    std::function<std::shared_ptr<WpaEvent>(const std::string_view message)> parse;
};

static const struct WpaEventMessageParser events[] = 
{
    {
        WPA_EVENT_CONNECTED,
        &parse_event_connected
    },
    {
        WPA_EVENT_DISCONNECTED,
        &parse_event_disconnected
    },
    {
        WPA_EVENT_ASSOC_REJECT,
        &parse_event_association_rejected
    },
    {
        WPA_EVENT_AUTH_REJECT,
        &parse_event_authentication_rejected
    },
    {
        WPA_EVENT_NETWORK_NOT_FOUND,
        &parse_event_network_not_found
    },
    {
        DPP_EVENT_AUTH_SUCCESS,
        &parse_event_dpp_auth_success
    },
    {
        DPP_EVENT_CONF_RECEIVED,
        &parse_event_dpp_conf_received
    },
    {
        DPP_EVENT_CONF_SENT,
        &parse_event_dpp_conf_sent
    },
    {
        DPP_EVENT_FAIL,
        &parse_event_dpp_fail
    },
    {
        DPP_EVENT_AUTH_INIT_FAILED,
        &parse_event_dpp_auth_init_fail
    },
    {
        DPP_EVENT_CHIRP_RX,
        &parse_event_dpp_chirp_rx
    },
    {
        DPP_EVENT_TX_STATUS,
        &parse_event_dpp_tx_status 
    }
};

std::shared_ptr<WpaEvent> 
WpaEventParser::parse(const char *buffer)
{
    std::string_view message(buffer);

    std::size_t priority_pos = message.find_first_of(">");
    if (priority_pos == std::string_view::npos) {
        std::cerr << "invalid wpa message (mising prefix)" << std::endl;
        return nullptr;
    }

    uint32_t priority;
    if (std::sscanf(message.data(), "<%" PRIu32 ">", &priority) != 1) {
        std::cerr << "unable to parse wpa message priority" << std::endl;
        return nullptr;
    }

    message.remove_prefix(priority_pos + 1);

    for (const auto& event : events) {
        std::size_t name_pos = message.find(event.prefix);
        if (name_pos != message.npos) {
            message.remove_prefix(name_pos + event.prefix.length());
            return event.parse(message);
        }
    }

    return nullptr;
}
