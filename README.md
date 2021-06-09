# Wi-Fi Telemetry

This project provides a wi-fi telemetry library and daemon. It enables realtime
collection and reporting of wi-fi related events. It can be used in conjunction
with the the [wifi-ztp](https://github.com/microsoft/wifi-ztp) project to
analyze and collect wi-fi zero touch provisioning telemetry as well.

## Building

### Ubuntu (focal)

```bash
sudo apt install build-essential cmake git liblttng-ust-dev libpci-dev libssl-dev libsystemd-dev pkg-config
```

Checkout and build:

```bash
git clone git@github.com:microsoft/wifi-telemetry.git
cd wifi-telemetry
mkdir build && cd $_
cmake ..
make -j $(nproc)
```

## Usage

The central concept is that of a telemetry monitor, represented as
[`WifiTelemetryMonitor`](include/wifi-telemetry/wifi_telemetry_monitor.hpp).
A monitor passively tracks one or more wi-fi telemetry sources, represented as
[`WifiTelemetrySource`](include/wifi-telemetry/wifi_telemetry_source.hpp),
each of which is optionally bound to a device interface (eg. `wlan0`) and wi-fi operational mode, either `station` or `access-point`. Event information is aggregated and translated to lttng events.

The daemon accepts a series of flag tuples for each telemetry source:

| Flag | Presence     | Description                  | Possible Values                        | Examples                 |
|------|--------------|------------------------------|----------------------------------------|--------------------------|
| `-s` | **Required** | telemetry source identifier  | `wpa`, `ztp`                           | `-s wpa`                 |
| `-i` | Optional     | wi-fi device interface name  | Any valid wif-device name              | `-i wlan0`, `-i  wl01s9` |
| `-o` | Optional     | wi-fi operational mode       | `sta`, `station`, `ap`, `access-point` | `-o station`, `-o ap`    |

### Telemetry Sources

#### Wi-Fi Protected Access (WPA) Supplicant (`wpa`)

This source passively monitors basic wi-fi connectivity and Device Provisioning Protocol (DPP aka [Wi-Fi EasyConnect](https://www.wi-fi.org/discover-wi-fi/wi-fi-easy-connect)) events originating from a wpa_supplicant control socket.

#### Zero Touch Provisioning Daemon (`ztp`)

This source passively monitors zero touch provisioning daemon (ztpd) events
originating from the ztpd d-bus inteface. Some events are not bound to a
specific interface.

### lttng Providers and Tracepoints

  | Provider       | Tracepoint                  | Source | Description                                          |
  |----------------|-----------------------------|--------|------------------------------------------------------|
  | `wifi`         | `device_info`               | `wpa`  | Interface, model, driver, subsystem and vendor info. |
  | `wifi-station` | `connection-attempt`        | `wpa`  | Result, bssid, signal, frequency, security, status.  |
  | `wifi-station` | `connection-drop`           | `wpa`  | Reason, bssid, locally generated.                    |
  | `wifi-dpp`     | `dpp_exchange_enrollee`     | `wpa`  | State, role, duration, failure details, frequencies. |
  | `wifi-dpp`     | `dpp_exchange_configurator` | `wpa`  | State, role, duration, failure details, frequency.   |
  | `wifi-dpp`     | `dpp_device_roles_changed`  | `ztp`  | Device role(s) changed.                              |

## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit [https://cla.opensource.microsoft.com](https://cla.opensource.microsoft.com).

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

## Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft 
trademarks or logos is subject to and must follow 
[Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks/usage/general).
Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship.
Any use of third-party trademarks or logos are subject to those third-party's policies.
