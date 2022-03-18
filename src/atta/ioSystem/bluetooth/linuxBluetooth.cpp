//--------------------------------------------------
// Atta IO System
// linuxBluetooth.cpp
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#if defined(ATTA_OS_LINUX) && defined(ATTA_SYSTEMD_SUPPORT)
#include <atta/ioSystem/bluetooth/linuxBluetooth.h>
// Thanks to https://github.com/infsoft-locaware/blzlib

namespace atta::io
{
    LinuxBluetooth::LinuxBluetooth(Bluetooth::CreateInfo info):
        Bluetooth(info), _bus(nullptr), _scanSlot(nullptr)
    {

    }

    LinuxBluetooth::~LinuxBluetooth()
    {
        stop();
    }

    bool LinuxBluetooth::start()
    {
        if(_bus && !stop())
        {
            LOG_ERROR(_debugName.getString(), "Could not stop previous system bus to start again");
            return false;
        }

        _adapter = "/org/bluez/hci0";

        int r;
        sd_bus_error error = SD_BUS_ERROR_NULL;

        // Connect to the system bus
        r = sd_bus_default_system(&_bus);
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to connect to system bus: [w]$0", strerror(-r));
            return false;
        }
        LOG_VERBOSE(_debugName.getString(), "Connected to system bus");

        // Power on if necessary
        r = sd_bus_set_property(_bus, "org.bluez", _adapter.c_str(), "org.bluez.Adapter1", "Powered", &error, "b", 1);
        if(r < 0)
        {
            if(sd_bus_error_has_name(&error, SD_BUS_ERROR_UNKNOWN_OBJECT))
                LOG_ERROR(_debugName.getString(), "Adapter [w]$0[] not known");
            else
                LOG_ERROR(_debugName.getString(), "Failed to power on adapter [w]$0[]: [w]$1", _adapter, error.message);
            sd_bus_error_free(&error);
            return false;
        }
        LOG_VERBOSE(_debugName.getString(), "Adapter powered on");

        sd_bus_error_free(&error);
        return true;
    }

    bool LinuxBluetooth::stop()
    {
        if(_scanSlot)
            stopScan();
        if(_bus)
            sd_bus_unref(_bus);
        _devices.clear();
        return true;
    }

    bool LinuxBluetooth::update()
    {
        if(!_bus)
        {
            LOG_WARN(_debugName.getString(), "Bluetooth must be started before updating");
            return false;
        }

        int r = sd_bus_process(_bus, NULL);
        if(r < 0) 
        {
            LOG_ERROR(_debugName.getString(), "Update failed to process system bus: [w]$0", strerror(-r));
            return false;
        }

        r = sd_bus_wait(_bus, 100);// Wait 0.1 ms
        if(r < 0 && -r != EINTR)
        {
            LOG_ERROR(_debugName.getString(), "Update failed to wait: [w]$0", strerror(-r));
            return false;
        }

        return true;
    }

    int LinuxBluetooth::bluezScanCallback(sd_bus_message* m, void* user, sd_bus_error* err)
    {
        LinuxBluetooth* linuxBluetooth = static_cast<LinuxBluetooth*>(user);
        if(linuxBluetooth == nullptr)
        {
            LOG_ERROR("io::LinuxBluetooth", "Scan callback to unknown instance");
            return -1;
        }

        const char* opath;

        // Read object path
        int r = sd_bus_message_read_basic(m, 'o', &opath);
        if(r < 0)
        {
            LOG_ERROR("io::LinuxBluetooth", "Failed to read bus message");
            return r;
        }

        // Check if the message is to this adapter
        std::string adapter = linuxBluetooth->getAdapter();
        if(strncmp(opath, adapter.c_str(), adapter.size()) == 0)
        {
            // Parse message
            return linuxBluetooth->bluezParseInterfaces(m, opath, MSG_DEVICE_SCAN);
        }
        else
        {
            // Skip message
            r = sd_bus_message_skip(m, "a{sa{sv}}");
            if(r < 0)
            {
                LOG_ERROR("io::LinuxBluetooth", "Failed to skip bus message");
                return r;
            }
            LOG_VERBOSE("io::LinuxBluetooth", "Skip message to $0", opath);
        }

        return 0;
    }

    bool LinuxBluetooth::startScan()
    {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        int r;

        r = sd_bus_match_signal(_bus, &_scanSlot, "org.bluez", "/",
                "org.freedesktop.DBus.ObjectManager",
                "InterfacesAdded", &atta::io::LinuxBluetooth::bluezScanCallback, this);

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to add bluez interface");
            return false;
        }

        r = sd_bus_call_method(_bus, "org.bluez", _adapter.c_str(),
                "org.bluez.Adapter1", "StartDiscovery", &error, NULL, "");

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to start scan: [w]$0", error.message);
            sd_bus_error_free(&error);
            return false;
        }

        LOG_VERBOSE(_debugName.getString(), "Started scanning...");
        sd_bus_error_free(&error);
        return true;
    }

    bool LinuxBluetooth::stopScan()
    {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        int r;
     
        r = sd_bus_call_method(_bus, "org.bluez", _adapter.c_str(),
                               "org.bluez.Adapter1", "StopDiscovery", &error, NULL, "");
     
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to stop scan: [w]$0", error.message);
            sd_bus_error_free(&error);
            return false;
        }
     
        _scanSlot = sd_bus_slot_unref(_scanSlot);

        sd_bus_error_free(&error);
        return true;
    }

    bool LinuxBluetooth::connect(std::array<uint8_t, 6> mac)
    {

    }
}
#include <atta/ioSystem/bluetooth/linuxBluetoothMsg.cpp>
#endif//ATTA_OS_LINUX && ATTA_SYSTEMD_SUPPORT 
