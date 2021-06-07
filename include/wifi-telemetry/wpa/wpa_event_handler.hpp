
#ifndef __WPA_EVENT_HANDLER_HPP__
#define __WPA_EVENT_HANDLER_HPP__

#include <chrono>
#include <memory>

#include "wifi-telemetry/wpa/wpa_events.hpp"

template <typename T>
struct WpaEventArgs
{
    WpaEventArgs(std::chrono::time_point<std::chrono::system_clock> timestamp_, std::shared_ptr<T> event_) :
        timestamp(timestamp_),
        event(event_)
    {}

    std::chrono::time_point<std::chrono::system_clock> timestamp;
    std::shared_ptr<T> event;

    const T&
    data() const
    {
        return *event;
    }
};

class WpaEventHandler
{
public:
    virtual void
    on_connected(const WpaEventArgs<WpaEventConnected>& args);

    virtual void
    on_disconnected(const WpaEventArgs<WpaEventDisconnected>& args);

    virtual void
    on_association_rejected(const WpaEventArgs<WpaEventAssociationRejected>& args);

    virtual void
    on_authentication_rejected(const WpaEventArgs<WpaEventAuthenticationRejected>& args);

    virtual void
    on_network_not_found(const WpaEventArgs<WpaEventNetworkNotFound>& args);

    virtual void
    on_dpp_chirp_received(const WpaEventArgs<WpaEventDppChirpReceived>& args);

    virtual void
    on_dpp_frame_transmit_status(const WpaEventArgs<WpaEventDppFrameTransmitStatus>& args);

    virtual void
    on_dpp_authentication_init_failure(const WpaEventArgs<WpaEventDppAuthenticationInitFailure>& args);
    
    virtual void
    on_dpp_authentication_succeeded(const WpaEventArgs<WpaEventDppAuthenticationSucceeded>& args);

    virtual void
    on_dpp_configuration_received(const WpaEventArgs<WpaEventDppConfigurationReceived>& args);

    virtual void
    on_dpp_configuration_sent(const WpaEventArgs<WpaEventDppConfigurationSent>& args);

    virtual void
    on_dpp_failure(const WpaEventArgs<WpaEventDppFailure>& args);
};

#endif //__WPA_EVENT_HANDLER_HPP__
