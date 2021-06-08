
#include "wifi-telemetry/wpa/wpa_event_handler.hpp"

void
WpaEventHandler::on_connected(const WpaEventArgs<WpaEventConnected>& /* args */)
{}

void
WpaEventHandler::on_disconnected(const WpaEventArgs<WpaEventDisconnected>& /* args */)
{}

void
WpaEventHandler::on_association_rejected(const WpaEventArgs<WpaEventAssociationRejected>& /* args */)
{}

void
WpaEventHandler::on_authentication_rejected(const WpaEventArgs<WpaEventAuthenticationRejected>& /* args */)
{}

void
WpaEventHandler::on_network_not_found(const WpaEventArgs<WpaEventNetworkNotFound>& /* args */)
{}

void
WpaEventHandler::on_dpp_authentication_init_failure(const WpaEventArgs<WpaEventDppAuthenticationInitFailure>& /* args */)
{}

void
WpaEventHandler::on_dpp_authentication_succeeded(const WpaEventArgs<WpaEventDppAuthenticationSucceeded>& /* args */)
{}

void
WpaEventHandler::on_dpp_configuration_received(const WpaEventArgs<WpaEventDppConfigurationReceived>& /* args */)
{}

void
WpaEventHandler::on_dpp_configuration_sent(const WpaEventArgs<WpaEventDppConfigurationSent>& /* args */)
{}

void
WpaEventHandler::on_dpp_failure(const WpaEventArgs<WpaEventDppFailure>& /* args */)
{}

void
WpaEventHandler::on_dpp_chirp_received(const WpaEventArgs<WpaEventDppChirpReceived>& /* args */)
{}

void
WpaEventHandler::on_dpp_frame_transmit_status(const WpaEventArgs<WpaEventDppFrameTransmitStatus>& /* args */)
{}
