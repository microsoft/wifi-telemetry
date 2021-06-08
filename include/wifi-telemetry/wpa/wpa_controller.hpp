
#ifndef __WPA_CONTROLLER_HPP__
#define __WPA_CONTROLLER_HPP__

#include <chrono>
#include <filesystem>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <wpa_ctrl.h>

#include "wifi-telemetry/wpa/wpa_command.hpp"
#include "wifi-telemetry/wpa/wpa_command_response.hpp"
#include "wifi-telemetry/wpa/wpa_event.hpp"
#include "wifi-telemetry/wpa/wpa_event_handler.hpp"
#include "wifi-telemetry/wpa/wpa_interface_info.hpp"

typedef uint32_t WpaEventToken;

class WpaController
{
public:
    WpaController(const std::string_view interface, WpaType type);
    WpaController(const std::string_view interface, WpaType type, const std::string_view control_socket_path);
    ~WpaController(void);

    /**
     * @brief Registers a handler for wpa events.
     * 
     * @param handler The handler to invoke in response to events.
     * @return WpaEventToken A registration token to be used to unregister the handler.
     */
    WpaEventToken
    register_for_events(std::weak_ptr<WpaEventHandler> handler);

    /**
     * @brief Unregisters an event handler for wpa events.
     * 
     * @param token The registration token previously obtains from register_for_events.
     */
    void
    unregister_for_events(WpaEventToken token);

    /**
     * @brief 
     * 
     * @param command 
     * @return std::shared_ptr<WpaControllerResponse> 
     */
    std::shared_ptr<WpaCommandResponse>
    send_command(const WpaCommand& command);

    /**
     * @brief Syntactic sugar for returning a specific derived type for the response.
     * 
     * @tparam TResponse 
     * @param command 
     * @return std::shared_ptr<TResponse> 
     */
    template <
        typename TResponse,
        typename std::enable_if<std::is_base_of<WpaCommandResponse, TResponse>::value>* = nullptr>
    std::shared_ptr<TResponse>
    send_command(const WpaCommand& command)
    {
        return std::dynamic_pointer_cast<TResponse>(send_command(command));
    }

    /**
     * @brief Returns the type of controller
     * 
     * @return const WpaType The type of controller.
     */
    WpaType
    type(void) const
    {
        return m_type;
    }

private:
    /**
     * @brief Retrieves the next available message from the control socket and
     * processes it.
     */
    void
    process_wpa_event_next(void);

    /**
     * @brief Waits for and processes all wpa control events.
     */
    void
    process_wpa_events(void);

    /**
     * @brief Get the command control socket object.
     * 
     * @return struct wpa_ctrl* 
     */
    struct wpa_ctrl*
    get_command_control_socket(void);

private:
    const WpaType m_type;
    std::string m_interface;
    std::filesystem::path m_ctrl_socket_path;
    std::thread m_ctrl_event_thread;
    bool m_terminate_pending = false;
    int m_ctrl_event_fd_stop = -1;
    struct wpa_ctrl* m_ctrl_event = nullptr;
    struct wpa_ctrl* m_ctrl_command = nullptr;
    std::shared_mutex m_ctrl_command_gate;

    uint32_t m_event_token_next = 0;
    std::mutex m_event_subscribers_gate;
    std::unordered_map<WpaEventToken, std::weak_ptr<WpaEventHandler>> m_event_subscribers;

private:
    /**
     * @brief Maximum wpas message size, in bytes. The wpa_cli tool uses this
     * as an upper bound so is used similarly here.
     */
    static constexpr std::size_t c_wpa_event_size_max = 4096;

    /**
     * @brief Default control socket pasth for wpa_supplicant.
     */
    static constexpr std::string_view m_ctrl_socket_path_default_wpas = std::string_view("/var/run/wpa_supplicant");

    /**
     * @brief Default control socket pasth for hostapd.
     */
    static constexpr std::string_view m_ctrl_socket_path_default_hostapd = std::string_view("/var/run/hostapd");

    /**
     * @brief Determines default control socket path for each wpa service type.
     * 
     * @param type The type of wpa service to determine the default control socket for.
     * @return constexpr std::string_view A string describing the default control sock path.
     */
    static constexpr std::string_view
    default_control_socket_path(WpaType type)
    {
        switch (type) {
            case WpaType::Hostapd:
                return m_ctrl_socket_path_default_hostapd;
            case WpaType::WpaSupplicant:
                return m_ctrl_socket_path_default_wpas;
            default:
                return "";
        }
    }
};

#endif //__WPA_CONTROLLER_HPP__
