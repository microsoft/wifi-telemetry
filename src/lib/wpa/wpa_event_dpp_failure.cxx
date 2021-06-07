
#include "wifi-telemetry/wpa/wpa_event_dpp_failure.hpp"

WpaEventDppFailure::WpaEventDppFailure(WpaEventType type_, WifiDppFailureType failure_type_, const std::string& failure_details_) :
    WpaEvent(type_),
    failure_type(failure_type_),
    failure_details(failure_details_)
{}

WpaEventDppFailure::WpaEventDppFailure(WifiDppFailureType failure_type_, const std::string& failure_details_) :
    WpaEventDppFailure(WpaEventType::DppFailure, failure_type_, failure_details_)
{}

WpaEventDppFailure::WpaEventDppFailure(const std::string& failure_details_) :
    WpaEventDppFailure(WifiDppFailureType::Unspecified, failure_details_)
{}
