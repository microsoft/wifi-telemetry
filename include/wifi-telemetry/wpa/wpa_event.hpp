
#ifndef __WPA_EVENT_HPP__
#define __WPA_EVENT_HPP__

enum WpaEventType
{
    Connected,
    Disconnected,
    NetworkNotFound,
    AssociationRejected,
    AuthenticationRejected,
    DppAuthenticationInitFailure,
    DppAuthenticationSucceeded,
    DppConfigurationReceived,
    DppConfigurationSent,
    DppFailure,
    DppFrameTransmitStatus,
    DppChirpReceived
};

struct WpaEvent
{
    WpaEvent(WpaEventType type);

    WpaEventType type;
    virtual ~WpaEvent() = default;
};

#endif //__WPA_EVENT_HPP__
