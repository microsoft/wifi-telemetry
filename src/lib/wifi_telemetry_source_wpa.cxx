
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "wifi-telemetry/wifi/wifi_80211_format_mac.hpp"
#include "wifi-telemetry/wifi_telemetry_source_wpa.hpp"
#include "wifi-telemetry/wpa/wpa_commands.hpp"
#include "wifi_telemetry_trace_dpp.hpp"
#include "wifi_telemetry_trace_station.hpp"

WifiTelemetrySourceWpa::WifiTelemetrySourceWpa(const std::optional<WifiInterfaceConfiguration>& interface) :
    WifiTelemetrySource(interface, "wpa")
{
    if (!interface.has_value())
        throw new std::invalid_argument("WifiTelemetrySourceWpa requires interface configuration, but none was supplied");
}

WifiTelemetrySourceWpa::~WifiTelemetrySourceWpa(void)
{
    if (m_controller) {
        m_controller->unregister_for_events(m_event_token);
        m_controller = nullptr;
    }
}

int
WifiTelemetrySourceWpa::activate(const std::shared_ptr<WifiTelemetrySourceActivationArgs> /* args */)
{
    WpaType type = WpaInterfaceInfo::TypeFromOperationalMode(m_interface->operational_mode);

    m_controller = std::make_unique<WpaController>(m_interface->name, type);
    m_event_token = m_controller->register_for_events(weak_from_this());

    return 0;
}

void
WifiTelemetrySourceWpa::deactivate(void)
{
    m_controller = nullptr;
}

bool
WifiTelemetrySourceWpa::trace_station(void) const
{
    return (m_interface->operational_mode == WifiOperationalMode::Station);
}

bool
WifiTelemetrySourceWpa::trace_ap(void) const
{
    return (m_interface->operational_mode == WifiOperationalMode::AccessPoint);
}

bool
WifiTelemetrySourceWpa::trace_dpp_enrollee(void) const
{
    // right now, assume all enrollees are wifi stations.
    return trace_station();
}

bool
WifiTelemetrySourceWpa::trace_dpp_configurator(void) const
{
    // right now, assume all configurators are wifi access points.
    return trace_ap();
}

bool
WifiTelemetrySourceWpa::trace_dpp(void) const
{
    return trace_dpp_enrollee() || trace_dpp_configurator();
}

bool
WifiTelemetrySourceWpa::is_dpp_exchange_in_progress(void) const
{
    return (m_dpp_exchange != nullptr);
}

void
WifiTelemetrySourceWpa::on_disconnected(const WpaEventArgs<WpaEventDisconnected>& args)
{
    if (!trace_station())
        return;

    const auto& event = args.data();
    std::string bssid = wifi_80211_mac_to_string(event.bssid);

    trace_station_connection_drop(
        m_interface->name.c_str(),
        bssid.c_str(),
        uint16_t(event.reason_code),
        event.locally_generated);
}

void
WifiTelemetrySourceWpa::complete_connection_attempt(const WifiTelemetryInputStationConnectionAttempt& input)
{
    trace_station_connection_attempt(m_interface->name.c_str(), &input);
}

void
WifiTelemetrySourceWpa::on_connected(const WpaEventArgs<WpaEventConnected>& args)
{
    if (!trace_station())
        return;

    const auto& event = args.data();

    struct WpaCommandSignalPoll signal_poll{};
    auto response_signal_poll = m_controller->send_command<WpaCommandSignalPollResponse>(signal_poll);
    if (!response_signal_poll) {
        std::cerr << "unable to obtain signal poll data" << std::endl;
        return;
    }

    struct WpaCommandStatus command_status{};
    auto response_status = m_controller->send_command<WpaCommandStatusResponse>(command_status);
    if (!response_status) {
        std::cerr << "unable to obtain interface status data" << std::endl;
        return;
    }

    auto connection_attempt_data = WifiTelemetryInputStationConnectionAttempt::succeeded(
        event.bssid,
        response_signal_poll,
        response_status);

    complete_connection_attempt(connection_attempt_data);
}

void
WifiTelemetrySourceWpa::on_association_rejected(const WpaEventArgs<WpaEventAssociationRejected>& args)
{
    if (!trace_station())
        return;

    const auto& event = args.data();
    auto connection_attempt_data = WifiTelemetryInputStationConnectionAttempt::association_rejected(
        event.bssid,
        event.status_code);

    complete_connection_attempt(connection_attempt_data);
}

void
WifiTelemetrySourceWpa::on_authentication_rejected(const WpaEventArgs<WpaEventAuthenticationRejected>& args)
{
    if (!trace_station())
        return;

    const auto& event = args.data();
    auto connection_attempt_data = WifiTelemetryInputStationConnectionAttempt::authentication_rejected(
        event.bssid,
        event.status_code,
        event.authentication_type);

    complete_connection_attempt(connection_attempt_data);
}

void
WifiTelemetrySourceWpa::on_network_not_found(const WpaEventArgs<WpaEventNetworkNotFound>& /* args */)
{
    if (!trace_station())
        return;

    auto connection_attempt_data = WifiTelemetryInputStationConnectionAttempt::network_not_found();
    complete_connection_attempt(connection_attempt_data);
}

void
WifiTelemetrySourceWpa::complete_dpp_exchange_enrollee(std::shared_ptr<WifiDppExchangeEnrollee>& enrollee)
{
    enrollee->stop();

    std::vector<uint32_t> chirp_frequencies(enrollee->chirp_frequencies.begin(), enrollee->chirp_frequencies.end());
    uint64_t duration_milliseconds = enrollee->duration.has_value()
        ? static_cast<uint64_t>(enrollee->duration->count())
        : 0;

    trace_dpp_exchange_enrollee(
        m_interface->name.c_str(),
        WifiDppRoleToString(enrollee->role),
        WifiDppExchangeEnrolleeStateToString(enrollee->state),
        duration_milliseconds,
        chirp_frequencies.size(),
        chirp_frequencies.data(),
        WifiDppFailureTypeToString(enrollee->failure_type),
        enrollee->failure_details.value_or("").c_str());

    m_dpp_exchange.reset();
}

void
WifiTelemetrySourceWpa::complete_dpp_exchange_configurator(std::shared_ptr<WifiDppExchangeConfigurator>& configurator)
{
    configurator->stop();

    std::string bssid = wifi_80211_mac_to_string(configurator->peer_bssid);
    uint64_t duration_milliseconds = configurator->duration.has_value()
        ? static_cast<uint64_t>(configurator->duration->count())
        : 0;

    trace_dpp_exchange_configurator(
        m_interface->name.c_str(),
        WifiDppRoleToString(configurator->role),
        WifiDppExchangeConfiguratorStateToString(configurator->state),
        duration_milliseconds,
        bssid.c_str(),
        configurator->frequency,
        WifiDppFailureTypeToString(configurator->failure_type),
        configurator->failure_details.value_or("").c_str());

    m_dpp_exchange.reset();
}

void
WifiTelemetrySourceWpa::on_dpp_chirp_received(const WpaEventArgs<WpaEventDppChirpReceived>& args)
{
    if (!trace_dpp_configurator())
        return;

    const auto& event = args.data();

    // configurator doesn't know about this device yet (id == -1), or another exchange in progress.
    if (event.id == -1 || is_dpp_exchange_in_progress())
        return;

    m_dpp_exchange = std::make_shared<WifiDppExchangeConfigurator>(event.id, event.bssid, event.frequency, WifiDppRole::Initiator);
}

void
WifiTelemetrySourceWpa::on_dpp_authentication_init_failure(const WpaEventArgs<WpaEventDppAuthenticationInitFailure>& args)
{
    if (!trace_dpp() || !is_dpp_exchange_in_progress())
        return;

    const auto& event = args.data();
    m_dpp_exchange->failure_type = event.failure_type;
    m_dpp_exchange->failure_details = event.failure_details;

    switch (m_dpp_exchange->device_role) {
        case WifiDppDeviceRole::Enrollee: {
            auto enrollee = resolve_dpp_exchange<WifiDppExchangeEnrollee>();
            enrollee->state = WifiDppExchangeEnrolleeState::Terminated;
            complete_dpp_exchange_enrollee(enrollee);
            break;
        }
        case WifiDppDeviceRole::Configurator: {
            auto configurator = resolve_dpp_exchange<WifiDppExchangeConfigurator>();
            configurator->state = WifiDppExchangeConfiguratorState::Terminated;
            complete_dpp_exchange_configurator(configurator);
            break;
        }
        default:
            return;
    }
}

void
WifiTelemetrySourceWpa::on_dpp_authentication_succeeded(const WpaEventArgs<WpaEventDppAuthenticationSucceeded>& /* args */)
{
    if (!trace_dpp() || !is_dpp_exchange_in_progress())
        return;

    switch (m_dpp_exchange->device_role) {
        case WifiDppDeviceRole::Enrollee: {
            auto enrollee = resolve_dpp_exchange<WifiDppExchangeEnrollee>();
            enrollee->state = WifiDppExchangeEnrolleeState::Authenticated;
            break;
        }
        case WifiDppDeviceRole::Configurator: {
            auto configurator = resolve_dpp_exchange<WifiDppExchangeConfigurator>();
            configurator->state = WifiDppExchangeConfiguratorState::Authenticated;
            break;
        }
        default:
            return;
    }
}

void
WifiTelemetrySourceWpa::on_dpp_configuration_received(const WpaEventArgs<WpaEventDppConfigurationReceived>& /* args */)
{
    if (!trace_dpp_enrollee() || !is_dpp_exchange_in_progress())
        return;

    if (m_dpp_exchange->device_role != WifiDppDeviceRole::Enrollee)
        return;

    auto enrollee = resolve_dpp_exchange<WifiDppExchangeEnrollee>();
    enrollee->state = WifiDppExchangeEnrolleeState::Provisioned;
    complete_dpp_exchange_enrollee(enrollee);
}

void
WifiTelemetrySourceWpa::on_dpp_configuration_sent(const WpaEventArgs<WpaEventDppConfigurationSent>& /* args */)
{
    if (!trace_dpp_configurator() || !is_dpp_exchange_in_progress())
        return;

    if (m_dpp_exchange->device_role != WifiDppDeviceRole::Configurator)
        return;

    auto configurator = resolve_dpp_exchange<WifiDppExchangeConfigurator>();
    configurator->state = WifiDppExchangeConfiguratorState::Finished;
    complete_dpp_exchange_configurator(configurator);
}

void
WifiTelemetrySourceWpa::on_dpp_failure(const WpaEventArgs<WpaEventDppFailure>& args)
{
    if (!trace_dpp() || !is_dpp_exchange_in_progress())
        return;

    const auto& event = args.data();
    m_dpp_exchange->failure_type = event.failure_type;
    m_dpp_exchange->failure_details = event.failure_details;

    switch (m_dpp_exchange->device_role) {
        case WifiDppDeviceRole::Enrollee: {
            auto enrollee = resolve_dpp_exchange<WifiDppExchangeEnrollee>();
            enrollee->state = WifiDppExchangeEnrolleeState::Terminated;
            complete_dpp_exchange_enrollee(enrollee);
            break;
        }
        case WifiDppDeviceRole::Configurator: {
            auto configurator = resolve_dpp_exchange<WifiDppExchangeConfigurator>();
            configurator->state = WifiDppExchangeConfiguratorState::Terminated;
            complete_dpp_exchange_configurator(configurator);
            break;
        }
        default:
            return;
    }
}

void
WifiTelemetrySourceWpa::on_dpp_frame_transmit_status(const WpaEventArgs<WpaEventDppFrameTransmitStatus>& args)
{
    if (!trace_dpp_enrollee())
        return;

    const auto& event = args.data();
    bool is_success = (event.status == "SUCCESS");
    bool is_broadcast = is_wifi80211_broadcast_address(event.destination_bssid);

    if (is_broadcast) { // this indicates a chirp
        if (!is_dpp_exchange_in_progress())
            m_dpp_exchange = std::make_shared<WifiDppExchangeEnrollee>(WifiDppRole::Responder);

        auto enrollee = resolve_dpp_exchange<WifiDppExchangeEnrollee>();
        if (is_success)
            enrollee->chirp_frequencies.insert(event.frequency);
    }
}
