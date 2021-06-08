
#ifndef __WIFI_TELEMETRY_MONITOR_HPP__
#define __WIFI_TELEMETRY_MONITOR_HPP__

#include <memory>
#include <type_traits>
#include <vector>

#include "wifi-telemetry/wifi_interface_configuration.hpp"
#include "wifi-telemetry/wifi_telemetry_source_instance.hpp"
#include "wifi-telemetry/wifi_telemetry_source.hpp"
#include "wifi-telemetry/wifi/wifi_common.hpp"

class __attribute__ ((visibility ("default"))) WifiTelemetryMonitor
{
public:
    /**
     * @brief Construct a new Wifi Telemetry Monitor object.
     */
    WifiTelemetryMonitor(void);
    ~WifiTelemetryMonitor(void);

    /**
     * @brief Adds a new telemetry source.
     * 
     * @tparam TSource The type of the telemetry source. Must be a type deriving from WifiTelemetrySource.
     * @tparam TSourceArgs The parameter pack type for T's constructor arguments.
     * @param interface The interface configuration of the source (if any).
     * @param activation_args The arguments needed to activate the telemetry source, or nullptr if none.
     * @param args  The arguments to forward to the constructor of type TSource.
     * @return std::shared_ptr<TSource> 
     */
    template <
        typename TSource, 
        typename ...TSourceArgs, 
        typename std::enable_if<std::is_base_of<WifiTelemetrySource, TSource>::value>* = nullptr
    >
    std::shared_ptr<TSource>
    add_source(const std::optional<WifiInterfaceConfiguration>& interface, const std::shared_ptr<WifiTelemetrySourceActivationArgs> activation_args, TSourceArgs... args)
    {
        std::shared_ptr<TSource> source = std::make_shared<TSource>(interface, args...);
        add_source_impl(source, activation_args);
        return source;
    }

    /**
     * @brief Adds a new telemetry source. This version assumes no activation args are required.
     * 
     * @tparam TSource The type of the telemetry source. Must be a type deriving from WifiTelemetrySource.
     * @tparam TSourceArgs The parameter pack type for T's constructor arguments.
     * @param interface The interface configuration of the source (if any).
     * @param args The arguments to forward to the constructor of type TSource.
     * @return std::shared_ptr<TSource> 
     */
    template <
        typename TSource, 
        typename ...TSourceArgs, 
        typename std::enable_if<std::is_base_of<WifiTelemetrySource, TSource>::value>* = nullptr
    >
    std::shared_ptr<TSource>
    add_source(const std::optional<WifiInterfaceConfiguration>& interface, TSourceArgs&&... args)
    {
        std::shared_ptr<TSource> source = std::make_shared<TSource>(interface, args...);
        add_source_impl(source);
        return source;
    }

    /**
     * @brief Removes a telemetry source from the monitor.
     * 
     * @param source A previously added telemetry source.
     */
    void
    remove_source(const std::shared_ptr<WifiTelemetrySource> source);

    /**
     * @brief Starts monitoring the configured interface.
     */
    void 
    start(void);

    /**
     * @brief Stops monitoring the configured interface.
     */
    void
    stop(void);

private:
    void
    add_source_impl(const std::shared_ptr<WifiTelemetrySource> source, const std::shared_ptr<WifiTelemetrySourceActivationArgs> activation_args = nullptr);

    std::size_t
    activate_sources(void);

    void
    deactivate_sources(void);

protected:
    bool m_activated = false;
    std::vector<std::unique_ptr<WifiTelemetrySourceInstance>> m_sources;
};

#endif //__WIFI_TELEMETRY_MONITOR_HPP__
