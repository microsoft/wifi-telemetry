
#include <cstdlib>
#include <filesystem>
#include <libudev.h>
#include <stdexcept>
#include <string.h>

extern "C" {
  #include <pci/pci.h>
}

#include "wifi_device_info.hpp"

WifiDeviceInfo::WifiDeviceInfo(
    const std::string& interface_,
    const std::string& bssid_,
    const std::string& subsystem_,
    const std::string& kernel_driver_,
    const std::string& kernel_driver_version_) :
interface(interface_),
bssid(bssid_),
subsystem(subsystem_),
kernel_driver(kernel_driver_),
kernel_driver_version(kernel_driver_version_)
{}

static uint16_t 
hexstr_to_uint16(const char *str)
{
    if (!str)
        return 0;

    unsigned long value = strtoul(str, nullptr, 16);
    if (value > UINT16_MAX)
        value = 0;
    
    return static_cast<uint16_t>(value);
}

static void 
fill_ids_pci(WifiDeviceInfo& device_info, struct udev_device *parent)
{
    const char *vendor_str = udev_device_get_sysattr_value(parent, "vendor");
    const char *device_str = udev_device_get_sysattr_value(parent, "device");
    const char *subsystem_device_str = udev_device_get_sysattr_value(parent, "subsystem_device");
    const char *subsystem_vendor_str = udev_device_get_sysattr_value(parent, "subsystem_vendor");

    uint16_t vendor = hexstr_to_uint16(vendor_str);
    uint16_t device = hexstr_to_uint16(device_str);
    uint16_t svendor = hexstr_to_uint16(subsystem_vendor_str);
    uint16_t sdevice = hexstr_to_uint16(subsystem_device_str); 

    struct pci_access *pacc = pci_alloc();
    char pci_vendor[128];
    char pci_device[128];
    char pci_svendor[128];
    char pci_sdevice[128];

    pci_init(pacc);

    device_info.vendor = pci_lookup_name(pacc, pci_vendor, sizeof pci_vendor, PCI_LOOKUP_VENDOR, vendor, device);
    device_info.device = pci_lookup_name(pacc, pci_device, sizeof pci_device, PCI_LOOKUP_DEVICE, vendor, device);
    device_info.subsystem_vendor = pci_lookup_name(pacc, pci_svendor, sizeof pci_svendor, PCI_LOOKUP_VENDOR | PCI_LOOKUP_SUBSYSTEM, svendor);
    device_info.subsystem_device = pci_lookup_name(pacc, pci_sdevice, sizeof pci_sdevice, PCI_LOOKUP_DEVICE | PCI_LOOKUP_SUBSYSTEM, vendor, device, svendor, sdevice);

    pci_cleanup(pacc);
}

static void
fill_ids_generic(WifiDeviceInfo& device_info, struct udev_device *parent)
{
    const char *vendor = udev_device_get_property_value(parent, "ID_VENDOR_FROM_DATABASE");
    const char *model = udev_device_get_property_value(parent, "ID_MODEL_FROM_DATABASE");

    if (!vendor) 
        vendor = udev_device_get_property_value(parent, "ID_VENDOR");
    if (!model)
        model = udev_device_get_property_value(parent, "ID_MODEL");

    if (vendor)
        device_info.vendor = vendor;
    if (model)
        device_info.device = model;
}

static void
fill_ids(WifiDeviceInfo& device_info, struct udev_device *parent)
{
    if (device_info.subsystem == "pci")
        fill_ids_pci(device_info, parent);
    else
        fill_ids_generic(device_info, parent);
}

WifiDeviceInfo 
WifiDeviceInfo::from_interface(const std::string& interface)
{
    const std::filesystem::path sysfs_path = std::filesystem::path("/sys/class/net") / interface;
    struct udev *udev = udev_new();
    struct udev_device *device = udev_device_new_from_syspath(udev, sysfs_path.c_str());
    if (!device)
        throw new std::invalid_argument(std::string("invalid sysfs path ") + sysfs_path.c_str());
    
    // Ensure this is a wireless device.
    const char *devtype = udev_device_get_devtype(device);
    if (!devtype || strcmp(devtype, "wlan") != 0)
        throw new std::invalid_argument(interface + " is not a wireless device");

    struct udev_device *device_parent = udev_device_get_parent(device);
    const char *kernel_driver_syspath = udev_device_get_syspath(device_parent);
    const std::filesystem::path sysfs_path_kmodule = std::filesystem::path(kernel_driver_syspath) / "driver/module";
    struct udev_device *device_module = udev_device_new_from_syspath(udev, sysfs_path_kmodule.c_str());
    if (!device_module)
        throw new std::invalid_argument("unable to obtain device kernel module");

    const char *address = udev_device_get_sysattr_value(device, "address");
    const char *subsystem = udev_device_get_subsystem(device_parent);
    const char *kernel_driver = udev_device_get_driver(device_parent);
    const char *kernel_driver_version = udev_device_get_sysattr_value(device_module, "version");

    WifiDeviceInfo device_info { interface, address, subsystem, kernel_driver, kernel_driver_version };

    fill_ids(device_info, device_parent);

    udev_device_unref(device_module);
    udev_device_unref(device_parent);
    udev_device_unref(device);
    udev_unref(udev);

    return device_info;
}
