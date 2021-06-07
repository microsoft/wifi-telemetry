
#ifndef __WIFI_80211_HPP__
#define __WIFI_80211_HPP__

#include <array>
#include <cstdint>

/**
 * @brief Unsolicited Management Frame reason code for Disassociation. 
 * @see IEEE Std 802.11-2016, Section 9.4.1.7, Table 9-45.
 */
enum class WifiDeauthenticationReasonCode : uint16_t
{
    Unspecified = 1,
    InvalidAuthentication = 2,
    LeavingNetworkDeauth = 3,
    Inactivity = 4,
    NoMoreStas = 5,
    InvalidClass2Frame = 6,
    InvalidClass3Frame = 7,
    LeavingNetworkDisassoc = 8,
    NotAuthenticated = 9,
    UnacceptablePowerCapability = 10,
    UnacceptableSupportedChannels = 11,
    BssTransitionDeassocac = 12,
    InvalidElement = 13,
    MicFailure = 14,
    FourWayHandshakeTimeout = 15,
    GroupKeyHandshakeTimeout = 16,
    HandshakeElementMismatch = 17,
    InvalidGroupCipher = 18,
    InvalidPairwiseCipher = 19,
    InvalidAkmp = 20,
    UnsupportedRsneVersion = 21,
    InvalidRsneCapabilities = 22,
    Ieee80211XAuthFailed = 23,
    CipherOutOfPolicy = 24,
    TdlsPeerUnreachable = 25,
    TdlsUnspecifiedReason = 26,
    SspRequestedDissassoc = 27,
    NoSspRoamingAgreement = 28,
    BadCipherOrAkm = 29,
    NotAuthorizedThisLocation = 30,
    ServiceChangePrecludesTs = 31,
    UnspecifiedQosReason = 32,
    NotEnoughBandwidth = 33,
    MissingAcks = 34,
    ExceededTxop = 35,
    StaLeaving = 36,
    EndTsBaDls = 37,
    UnknownTsBa = 38,
    Timeout = 39,
    PeerkeyMismatch = 45,
    PeerInitiated = 46,
    ApInitiated = 47,
    InvalidFtActionFrameCount = 48,
    InvalidPmkid = 49,
    InvalidMde = 50,
    InvalidFte = 51,
    MeshPeeringCanceled = 52,
    MeshMaxPeers = 53,
    MeshConfigurationPolicyViolation = 54,
    MeshCloseRecvd = 55,
    MeshMaxRetries = 56,
    MeshConfirmTimeout = 57,
    MeshInvalidGtk = 58,
    MeshInconsistentParameters = 59,
    MeshInvalidSecurityCapability = 60,
    MeshPathErrorNoProxyInformation = 61,
    MeshPathErrorNoForwardingInformation = 62,
    MeshPathErrorDestinationUnreachable = 63,
    MaxAddressAlreadyExistsInMbss = 64,
    MeshChannelSwitchRegulatoryRequirements = 65,
    MeshChannelSwitchUnspecified = 66,
};

/**
 * @brief Unsolicited Management Frame status code. 
 * @see IEEE Std 802.11-2016, Section 9.4.1.9, Table 9-46.
 */
enum class WifiStatusCode : uint16_t
{
    Success = 0,
    Refused = 1,
    TdlsRejectedAlternativeProvided = 2,
    TdlsRejected = 3,
    /* reserved = 4 */
    SecurityDisabled = 5,
    UnacceptableLifetime = 6,
    NotInSameBss = 7,
    /* reserved = 8-9 */
    RefusedCapabilitiesMismatch = 10,
    DeniedNoAssociationExists = 11,
    DeniedOtherReason = 12,
    UnsupportedAuthAlgorithm = 13,
    TransactionSequenceError = 14,
    ChallengeFailure = 15,
    RejectedSequenceTimeout = 16,
    DeniedNoMoreStas = 17,
    RefusedBasicRatesMismatch = 18,
    DeniedNoShortPreableSupport = 19,
    /* reserved = 20-21 */
    RejectedSpectrumManagementRequired = 22,
    RejectedBadPowerCapability = 23,
    RejectedBadSupportedChannels = 24,
    DeniedNoShortSlotTimeSupport = 25,
    /* reserved = 26 */
    DeniedNoHtSupport = 27,
    R0KHUnreachable = 28,
    DeniedPcoTimeNotSupported = 29,
    RefusedTemporarily = 30,
    RobustManagementPolicyViolation = 31,
    UnspecifiedQosFailure = 32,
    DeniedInsufficientBandwidth = 33,
    DeniedPoorChannelConditions = 34,
    DeniedQosNotSupported = 35,
    /* reserved = 36 */
    RequestDeclined = 37,
    InvalidParameters = 38,
    RejectedWithSuggestedChanges = 39,
    StatusInvalidElement = 40,
    StatusInvalidGroupCipher = 41,
    StatusInvalidPairwiseCipher = 42,
    StatusInvalidAkmp = 43,
    UnsupportedRsneVersion = 44,
    InvalidRsneCapabilities = 45,
    StatusCipherOutOfPolicy = 46,
    RejectedForDelayPeriod = 47,
    DlsNotAllowed = 48,
    NotPresent = 49,
    NotQosSta = 50,
    DeniedListenIntervalTooLarge = 51,
    StatusInvalidFtActionFrameCount = 52,
    StatusInvalidPmkid = 53,
    StatusInvalidMde = 54,
    StatusInvalidFte = 55,
    RequestTclasNotSupported = 56,
    InsufficientTclasProcessingResources = 57,
    TryAnotherBss = 58,
    GasAdvertisementProtocolNotSupported = 59,
    NoOutstandingGasRequest = 60,
    GasResponseNotReceivedFromServer = 61,
    GasQueryTimeout = 62,
    GasQueryResponseTooLarge = 63,
    RejectedHomeWithSuggestedChanges = 64,
    ServerUnreachable = 65,
    /* reserved = 66 */
    RejectedForSspPermissions = 67,
    RefusedUnauthenticatedAccessNotSupported = 68,
    /* reserved = 69-71 */
    InvalidRsne = 72,
    UApsdCoexistanceNotSupported = 73,
    UApsdCoexModeNotSupported = 74,
    BadIntervalWithUApsdCoex = 75,
    AntiCloggingTokenRequired = 76,
    UnsupportedFiniteCyclicGroup = 77,
    CannotFindAlternativeTbtt = 78,
    TransmissionFailure = 79,
    RequestTclassNotSupported = 80,
    TclasResourcesExhausted = 81,
    RejectedWithSuggestedBssTransition = 82,
    RejectWithSchedule = 83,
    RejectNoWakeupSpecified = 84,
    SuccessPowerSaveMode = 85,
    PendingAdmittingFstSession = 86,
    PerformingFstNow = 87,
    PendingGapInBaWindow = 88,
    RejectUPidSetting = 89,
    /* reserved = 90-91 */
    RefusedExternalReason = 92,
    RefusedApOutOutMemory = 93,
    RejectedEmergencyServicesNotSupported = 94,
    QueryResponseOutstanding = 95,
    RejectBadDseBand = 96,
    TclasProcessingTerminated = 97,
    TsScheduleConflict = 98,
    DeniedWithSuggestedBandAndChannel = 99,
    MccaopReservationConflict = 100,
    MafLimitExceeded = 101,
    MccaTrackLimitExceeded = 102,
    DeniedDueToSpectrumManagement = 103,
    DeniedVhtNotSupported = 104,
    EnablementDenied = 105,
    RestrictionFromAuthorixzedGdb = 106,
    AuthorizationDeenabled = 107,
    /* reserved = 108-165, 535 */
};

enum class WifiAuthenticationType : uint32_t
{
    Unknown = 0x0000,
    Open = 0x0001,
    WpaPsk = 0x0002,
    Shared = 0x0004,
    Wpa = 0x0008,
    Wpa2 = 0x0010,
    Wpa2Psk = 0x0020,
};

inline const char*
WifiAuthenticationTypeToString(const WifiAuthenticationType auth_type)
{
    static const char* strs[] = {
        "unknown",
        "open",
        "wpa-psk",
        "shared",
        "wpa",
        "wpa2",
        "wpa2-psk"
    };

    return strs[unsigned(auth_type)];
}

typedef std::array<uint8_t, 6> wifi_80211_mac;

#define WIFI_80211_MAC_FMT "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx"
#define STR_2_WIFI80211_MAC(a) &a[0], &a[1], &a[2], &a[3], &a[4], &a[5]

static const wifi_80211_mac wifi_80211_any_address{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static const wifi_80211_mac wifi_80211_broadcast_address{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

static inline bool
is_wifi80211_broadcast_address(const wifi_80211_mac& mac)
{
    return mac == wifi_80211_broadcast_address;
}

#endif //__WIFI_80211_HPP__
