
#include <array>
#include <cerrno>
#include <filesystem>
#include <iostream>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <unistd.h>

#include "wifi-telemetry/wpa/wpa_controller.hpp"
#include "wpa_event_parser.hpp"

WpaController::WpaController(const std::string_view interface, WpaType type) :
    WpaController(interface, type, WpaController::default_control_socket_path(type))
{
}

WpaController::WpaController(const std::string_view interface, WpaType type, const std::string_view control_socket_path) :
    m_type(type),
    m_interface(interface),
    m_ctrl_socket_path(control_socket_path)
{
    m_ctrl_event_thread = std::thread([this](void)
    {
        process_wpa_events();
    });
}

WpaController::~WpaController(void)
{
    m_terminate_pending = true;

    if (m_ctrl_event_fd_stop != -1) {
        uint64_t value = 1;
        ssize_t num_written = write(m_ctrl_event_fd_stop, &value, sizeof value);
        if (num_written != sizeof value) {
            int ret = (num_written < 0) ? errno : -ERANGE;
            std::cerr << "failed to write exit signal descriptor value (" << ret << ")" << std::endl;
        }
    }

    auto lock_exclusive = std::scoped_lock(m_ctrl_command_gate);
    if (m_ctrl_command) {
        wpa_ctrl_close(m_ctrl_command);
        m_ctrl_command = nullptr;
    }

    m_ctrl_event_thread.join();
}

/**
 * @brief Process a pending wpa event.
 */
void
WpaController::process_wpa_event_next(void)
{
    auto timestamp = std::chrono::system_clock::now();
    char buffer[WpaController::c_wpa_event_size_max];
    size_t buffer_length = sizeof buffer - 1;

    int ret = wpa_ctrl_recv(m_ctrl_event, buffer, &buffer_length);
    if (ret < 0) {
        std::cerr << "failed to receive message from wpa control socket" << std::endl;
        return;
    }

    buffer[buffer_length] = '\0';

    std::shared_ptr<WpaEvent> event = WpaEventParser::parse(buffer);
    if (!event)
        return;

    // Make a copy of the event subscribers to minimize the time holding the
    // lock. This is safe since the validity of each subscriber will later be
    // attempted to be promoted to a full shared_ptr prior to using it.
    decltype(m_event_subscribers) event_subscribers;
    {
        auto lock = std::scoped_lock(m_event_subscribers_gate);
        event_subscribers = m_event_subscribers;
    }

    for (const auto& [token, handler_weak] : event_subscribers) {
        std::shared_ptr<WpaEventHandler> handler = handler_weak.lock();
        if (!handler)
            continue;

        switch (event->type) {
            case WpaEventType::Connected:
                handler->on_connected({ timestamp, std::dynamic_pointer_cast<WpaEventConnected>(event) });
                break;
            case WpaEventType::Disconnected:
                handler->on_disconnected({ timestamp, std::dynamic_pointer_cast<WpaEventDisconnected>(event) });
                break;
            case WpaEventType::AssociationRejected:
                handler->on_association_rejected({ timestamp, std::dynamic_pointer_cast<WpaEventAssociationRejected>(event) });
                break;
            case WpaEventType::AuthenticationRejected:
                handler->on_authentication_rejected({ timestamp, std::dynamic_pointer_cast<WpaEventAuthenticationRejected>(event) });
                break;
            case WpaEventType::NetworkNotFound:
                handler->on_network_not_found({ timestamp, std::dynamic_pointer_cast<WpaEventNetworkNotFound>(event) });
                break;
            case WpaEventType::DppAuthenticationInitFailure:
                handler->on_dpp_authentication_init_failure({ timestamp, std::dynamic_pointer_cast<WpaEventDppAuthenticationInitFailure>(event) });
                break;
            case WpaEventType::DppAuthenticationSucceeded:
                handler->on_dpp_authentication_succeeded({ timestamp, std::dynamic_pointer_cast<WpaEventDppAuthenticationSucceeded>(event) });
                break;
            case WpaEventType::DppConfigurationReceived:
                handler->on_dpp_configuration_received({ timestamp, std::dynamic_pointer_cast<WpaEventDppConfigurationReceived>(event) });
                break;
            case WpaEventType::DppConfigurationSent:
                handler->on_dpp_configuration_sent({ timestamp, std::dynamic_pointer_cast<WpaEventDppConfigurationSent>(event) });
                break;
            case WpaEventType::DppFailure:
                handler->on_dpp_failure({ timestamp, std::dynamic_pointer_cast<WpaEventDppFailure>(event) });
                break;
            case WpaEventType::DppChirpReceived:
                handler->on_dpp_chirp_received({ timestamp, std::dynamic_pointer_cast<WpaEventDppChirpReceived>(event) });
                break;
            case WpaEventType::DppFrameTransmitStatus:
                handler->on_dpp_frame_transmit_status({ timestamp, std::dynamic_pointer_cast<WpaEventDppFrameTransmitStatus>(event) });
                break;
        }
    }
}

void
WpaController::process_wpa_events(void)
{
    static constexpr int epoll_events_max = 2;

    int ret;
    int fd_efd = -1;
    int fd_wpa = -1;
    bool terminate = false;
    struct wpa_ctrl* ctrl = nullptr;
    struct epoll_event events[epoll_events_max] = {};
    struct epoll_event* event_efd = &events[0];
    struct epoll_event* event_wpa = &events[1];
    const std::filesystem::path path = m_ctrl_socket_path / m_interface;

    int fd_epoll = epoll_create1(0);
    if (fd_epoll < 0) {
        ret = errno;
        std::cerr << "failed to create epoll instance for wpa event listener (" << ret << ")" << std::endl;
        return;
    }

    // configure eventfd descriptor for thread stop signaling.
    fd_efd = eventfd(0, 0);
    if (fd_efd == -1) {
        ret = errno;
        std::cerr << "failed to create stop event fd (" << ret << ")" << std::endl;
        goto cleanup;
    }

    event_efd->events = EPOLLIN;
    event_efd->data.fd = fd_efd;

    ret = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_efd, event_efd);
    if (ret < 0) {
        ret = errno;
        std::cerr << "failed to register epoll handler for event stop signaling (" << ret << ")" << std::endl;
        goto cleanup;
    }

    // configure control socket descriptor for wpa event signaling.
    ctrl = wpa_ctrl_open(path.c_str());
    if (!ctrl) {
        std::cerr << "failed to open wpa control socket " << path << std::endl;
        goto cleanup;
    }

    fd_wpa = wpa_ctrl_get_fd(ctrl);
    event_wpa->events = EPOLLIN;
    event_wpa->data.fd = fd_wpa;

    ret = epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd_wpa, event_wpa);
    if (ret < 0) {
        ret = errno;
        std::cerr << "failed to register epoll handler for wpa event listener (" << ret << ")" << std::endl;
        goto cleanup;
    }

    // attach to wpa event stream
    ret = wpa_ctrl_attach(ctrl);
    if (ret < 0) {
        std::cerr << "failed to register event monitor on wpa control socket (" << ret << ")" << std::endl;
        goto cleanup;
    }

    m_ctrl_event = ctrl;
    m_ctrl_event_fd_stop = fd_efd;

    while (!m_terminate_pending && !terminate) {
        // wait for at least one event fd to be signaled
        int num_events = epoll_wait(fd_epoll, events, epoll_events_max, -1);
        if (num_events < 0) {
            ret = errno;
            std::cerr << "failed to wait on epoll event (" << ret << ")" << std::endl;
            continue;
        } else if (num_events == 0) {
            continue;
        }

        // determine which fd(s) were signaled and handle them.
        for (int i = 0; i < epoll_events_max; i++) {
            if (events[i].data.fd == fd_efd) {
                uint64_t value;
                ssize_t num_read = read(fd_efd, &value, sizeof value);
                if (num_read != sizeof value) {
                    ret = errno;
                    std::cerr << "failed to read exit signal descriptor (" << ret << ")" << std::endl;
                    continue;
                } else if (value > 0) {
                    terminate = true;
                }
            } else if (events[i].data.fd == fd_wpa) {
                while (wpa_ctrl_pending(ctrl) > 0)
                    process_wpa_event_next();
            }
        }
    }

cleanup:
    if (ctrl) {
        wpa_ctrl_detach(ctrl);
        wpa_ctrl_close(ctrl);
    }

    if (fd_efd != -1)
        close(fd_efd);

    close(fd_epoll);

    m_ctrl_event_fd_stop = -1;
    m_ctrl_event = nullptr;
}

WpaEventToken
WpaController::register_for_events(std::weak_ptr<WpaEventHandler> handler)
{
    auto lock = std::scoped_lock(m_event_subscribers_gate);
    WpaEventToken token = m_event_token_next++;
    m_event_subscribers[token] = handler;

    return token;
}

void
WpaController::unregister_for_events(WpaEventToken token)
{
    auto lock = std::scoped_lock(m_event_subscribers_gate);
    m_event_subscribers.erase(token);
}

struct wpa_ctrl*
WpaController::get_command_control_socket(void)
{
    const std::filesystem::path path = m_ctrl_socket_path / m_interface;

    // Check if the socket is already available.
    {
        auto lock_shared = std::shared_lock(m_ctrl_command_gate);
        if (m_ctrl_command)
            return m_ctrl_command;
    }

    // Check if socket was updated between releasing shared lock and acquiring exclusive lock.
    auto lock_exclusive = std::scoped_lock(m_ctrl_command_gate);
    if (m_ctrl_command)
        return m_ctrl_command;

    struct wpa_ctrl* ctrl = wpa_ctrl_open(path.c_str());
    if (!ctrl) {
        std::cerr << "failed to open wpa control socket " << path << std::endl;
        return nullptr;
    }

    m_ctrl_command = ctrl;
    return m_ctrl_command;
}

std::shared_ptr<WpaCommandResponse>
WpaController::send_command(const WpaCommand& command)
{
    struct wpa_ctrl* ctrl = get_command_control_socket();
    if (!ctrl) {
        std::cerr << "failed to obtain wpa command control socket" << std::endl;
        return nullptr;
    }

    std::array<char, c_wpa_event_size_max> reply;
    std::size_t reply_length = reply.size();

    int ret = wpa_ctrl_request(ctrl, command.data.c_str(), command.data.length(), reply.data(), &reply_length, nullptr);
    switch (ret) {
        case 0:
            return command.parse_response(std::string_view(reply.data(), reply_length));
        case -2:
            std::cerr << "failed to send/receive wpa command " << command.name << "(timeout)" << std::endl;
            return nullptr;
        case -1:
            std::cerr << "failed to send/receive wpa command " << command.name << std::endl;
            return nullptr;
        default:
            std::cerr << "failed to send/receive wpa command " << command.name << "(unspecified error " << ret << ")" << std::endl;
            return nullptr;
    }
}
