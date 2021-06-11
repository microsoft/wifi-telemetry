# Wi-Fi Telemetry

This project provides a Wi-Fi telemetry library and daemon. It enables realtime
collection and reporting of Wi-Fi related events. It can be used in conjunction
with the the [wifi-ztp](https://github.com/microsoft/wifi-ztp) project to
analyze and collect Wi-Fi zero touch provisioning telemetry as well.

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
A monitor passively tracks one or more Wi-Fi telemetry sources, represented as
[`WifiTelemetrySource`](include/wifi-telemetry/wifi_telemetry_source.hpp),
each of which is optionally bound to a device interface (eg. `wlan0`) and Wi-Fi operational mode, either `station` or `access-point`. Event information is aggregated and translated to [lttng](https://lttng.org/) events. All telemetry is local and ***never leaves the device***.

The daemon accepts a series of flag tuples for each telemetry source:

| Flag | Presence     | Description                  | Supported Values                       | Examples                 |
|------|--------------|------------------------------|----------------------------------------|--------------------------|
| `-s` | **Required** | telemetry source identifier  | `wpa`, `ztp`                           | `-s wpa`                 |
| `-i` | Optional     | Wi-Fi device interface name  | Any valid Wi-Fi device name            | `-i wlan0`, `-i  wl01s9` |
| `-o` | Optional     | Wi-Fi operational mode       | `sta`, `station`, `ap`, `access-point` | `-o station`, `-o ap`    |

### Telemetry Sources

#### Wi-Fi Protected Access (WPA) Supplicant (`wpa`)

This source passively monitors basic Wi-Fi connectivity and Device Provisioning Protocol (DPP aka [Wi-Fi EasyConnect](https://www.wi-fi.org/discover-wi-fi/wi-fi-easy-connect)) events originating from a wpa_supplicant control socket.

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

### Examples

#### Monitor basic Wi-Fi client connectivity on `wlan0`

```shell
$ wifi-telemetryd -s wpa -i wlan0 -s sta
activated telemetry source 'wpa'
telemetry monitor started with 1 of 1 telemetry sources
```

#### Monitor basic wifi-client connectivity and ztp events on `wlan1`

```shell
$ wifi-telemetryd -s wpa -i wlan1 -s station -s ztp
activated telemetry source 'wpa'
activated telemetry source 'ztpd'
telemetry monitor started with 2 of 2 telemetry sources
```

#### Monitor Wi-Fi client connectivity with systemd instantiated service unit on arbitrary interface (`%i`)

[wifi-telemetryd-station@service.in](src/daemon/systemd/wifi-telemetryd-station@.service.in):

```ini
[Unit]
Description=Wi-Fi Telemetry Daemon (WPA Station)
Requires=sys-subsystem-net-devices-%i.device
After=sys-subsystem-net-devices-%i.device

[Service]
Type=simple
Restart=on-failure
ExecStart=/usr/sbin/wifi-telemetryd -s wpa -o station -i %i
```

#### View all Wi-Fi telemetry in real-time with lttng

```shell
$ lttng create --live
Live session auto-20210610-220234 created.
Traces will be output to tcp4://127.0.0.1:5342/ [data: 5343]
Live timer interval set to 1000000 us
$ lttng enable-event -u "wifi:* wifi_station:* wifi_dpp:*"
UST event wifi:* wifi_station:* wifi_dpp:* created in channel channel0
$ lttng start
Tracing started for session auto-20210610-220234
$ lttng view
Trace directory: net://localhost/host/apd-d8c0a65935ed/auto-20210610-220234$ lttng create --live
<Wi-Fi tracepoint events...>
```

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
