
#ifndef __WIFI_DPP_HPP__
#define __WIFI_DPP_HPP__

enum class WifiDppRole
{
    Initiator,
    Responder
};

inline const char*
WifiDppRoleToString(const WifiDppRole role)
{
    static const char* strs[] = {
        "initiator",
        "responder"
    };

    return strs[unsigned(role)];
}

enum class WifiDppDeviceRole
{
    Enrollee,
    Configurator
};

inline const char*
WifiDppDeviceRoleToString(const WifiDppDeviceRole role)
{
    static const char* strs[] = {
        "enrollee",
        "configurator"
    };

    return strs[unsigned(role)];
}

enum class WifiDppExchangeEnrolleeState
{
    Bootstrapping,
    Authenticated,
    Provisioning,
    Provisioned,
    Terminated
};

inline const char*
WifiDppExchangeEnrolleeStateToString(const WifiDppExchangeEnrolleeState state)
{
    static const char* strs[] = {
        "bootstrapping",
        "authenticated",
        "provisioning",
        "provisioned",
        "terminated"
    };

    return strs[unsigned(state)];
}

enum class WifiDppExchangeConfiguratorState
{
    Bootstrapping,
    Authenticated,
    Finished,
    Terminated
};

inline const char*
WifiDppExchangeConfiguratorStateToString(const WifiDppExchangeConfiguratorState state)
{
    static const char* strs[] = {
        "bootstrapping",
        "authenticated",
        "finished",
        "terminated"
    };

    return strs[unsigned(state)];
}

enum class WifiDppFailureType
{
    None,
    Unspecified,
    AuthenticationTimeout,
};

inline const char*
WifiDppFailureTypeToString(const WifiDppFailureType type)
{
    static const char* strs[] = {
        "none",
        "unspecified",
        "authentication-timeout",
    };

    return strs[unsigned(type)];
}

#endif //__WIFI_DPP_HPP__
