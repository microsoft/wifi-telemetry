
#ifndef __WIFI_TELEMETRY_SOURCE_WPA_HPP__
#define __WIFI_TELEMETRY_SOURCE_WPA_HPP__

#include <memory>
#include <optional>
#include <string>

#include "wifi-telemetry/wifi/wifi_common.hpp"
#include "wifi-telemetry/wifi/wifi_dpp_exchange.hpp"
#include "wifi-telemetry/wifi_telemetry_source.hpp"
#include "wifi-telemetry/wpa/wifi_telemetry_input_station_connection_attempt.hpp"
#include "wifi-telemetry/wpa/wpa_controller.hpp"

class __attribute__((visibility("default"))) WifiTelemetrySourceWpa :
    public WifiTelemetrySource,
    public std::enable_shared_from_this<WifiTelemetrySourceWpa>,
    public WpaEventHandler
{
public:
    WifiTelemetrySourceWpa(const std::optional<WifiInterfaceConfiguration>& interface);
    ~WifiTelemetrySourceWpa(void);

    int
    activate(const std::shared_ptr<WifiTelemetrySourceActivationArgs> args) override;

    void
    deactivate(void) override;

private:
    bool
    trace_station(void) const;

    bool
    trace_ap(void) const;

    bool
    trace_dpp(void) const;

    bool
    trace_dpp_enrollee(void) const;

    bool
    trace_dpp_configurator(void) const;

    // WpaEventHandler
    void
    on_connected(const WpaEventArgs<WpaEventConnected>& args) override;

    void
    on_disconnected(const WpaEventArgs<WpaEventDisconnected>& args) override;

    void
    on_association_rejected(const WpaEventArgs<WpaEventAssociationRejected>& args) override;

    void
    on_authentication_rejected(const WpaEventArgs<WpaEventAuthenticationRejected>& args) override;

    void
    on_network_not_found(const WpaEventArgs<WpaEventNetworkNotFound>& args) override;

    void
    on_dpp_chirp_received(const WpaEventArgs<WpaEventDppChirpReceived>& args) override;

    void
    on_dpp_frame_transmit_status(const WpaEventArgs<WpaEventDppFrameTransmitStatus>& args) override;

    void
    on_dpp_authentication_init_failure(const WpaEventArgs<WpaEventDppAuthenticationInitFailure>& args) override;
    
    void
    on_dpp_authentication_succeeded(const WpaEventArgs<WpaEventDppAuthenticationSucceeded>& args) override;

    void
    on_dpp_configuration_received(const WpaEventArgs<WpaEventDppConfigurationReceived>& args) override;

    void
    on_dpp_configuration_sent(const WpaEventArgs<WpaEventDppConfigurationSent>& args) override;

    void
    on_dpp_failure(const WpaEventArgs<WpaEventDppFailure>& args) override;

private:
    template<
        typename T,
        typename std::enable_if<std::is_base_of<WifiDppExchange, T>::value>* = nullptr>
    std::shared_ptr<T>
    resolve_dpp_exchange(void)
    {
        return std::dynamic_pointer_cast<T>(m_dpp_exchange);
    }

    bool
    is_dpp_exchange_in_progress(void) const;

    void
    complete_connection_attempt(const WifiTelemetryInputStationConnectionAttempt& input);

    void
    complete_dpp_exchange_enrollee(std::shared_ptr<WifiDppExchangeEnrollee>& enrollee);

    void
    complete_dpp_exchange_configurator(std::shared_ptr<WifiDppExchangeConfigurator>& configurator);

    std::shared_ptr<WifiDppExchange> m_dpp_exchange = nullptr;
    std::shared_ptr<WpaController> m_controller;
    WpaEventToken m_event_token;
};

#endif //__WIFI_TELEMETRY_SOURCE_WPA_HPP__
