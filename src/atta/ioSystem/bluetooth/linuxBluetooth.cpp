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
        // Clear callback payload
        for(auto& cPayload : _callbackPayloads)
        {
            cPayload.linuxBluetooth = nullptr;
            std::fill(std::begin(cPayload.mac), std::end(cPayload.mac), 0);
        }
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

        // Populate known devices
        populateKnownDevices();

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

    bool LinuxBluetooth::populateKnownDevices()
    {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        sd_bus_message* reply = NULL;   
        int r;                

        r = sd_bus_call_method(_bus, "org.bluez", "/",
                "org.freedesktop.DBus.ObjectManager",
                "GetManagedObjects", &error, &reply, "");

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to get managed objecs (known devices): [w]$0", error.message);
            sd_bus_error_free(&error);
            sd_bus_message_unref(reply);    
            return false;
        }

        r = bluezParseObjects(reply, _adapter.c_str(), MSG_DEVICE_SCAN);

        sd_bus_error_free(&error);
        sd_bus_message_unref(reply);    
        return true;
    }

    //----------------------------------------//
    //----------------- Scan -----------------//
    //----------------------------------------//
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

    //----------------------------------------//
    //--------------- Connect ----------------//
    //----------------------------------------//
    int LinuxBluetooth::bluezConnectCallback(sd_bus_message* m, void* user, sd_bus_error* err)
    {
        LOG_INFO("io::LinuxBluetooth", "Connect callback");
        //----- Unpack payload -----//
        CallbackPayload* cPayload = static_cast<CallbackPayload*>(user);
        LinuxBluetooth* linuxBluetooth = nullptr;
        LinuxDevice* lDev = nullptr;
        Device* dev = nullptr;
        if(unpackCallbackPayload(cPayload, &linuxBluetooth, &lDev, &dev) < 0)
            return -1;

        //----- Success -----//
        linuxBluetooth->bluezParseInterface(m, nullptr, MSG_DEVICE, lDev);
        LOG_SUCCESS("io::LinuxBluetooth", "Connected to [w]$0[]", MACToString(lDev->mac));

        return 0;
    }

    bool LinuxBluetooth::connect(std::array<uint8_t, 6> mac)
    {
        int r;
        sd_bus_error error = SD_BUS_ERROR_NULL;
        enum ConnStatus
        {
            UNDEFINED = 0,
            CONNECTED,
            KNOWN_DEVICE_NOT_CONNECTED,
            UNKNOWN_DEVICE_NOT_CONNECTED
        };
        ConnStatus connStatus = UNDEFINED;

        Device* dev = nullptr;
        LinuxDevice* lDev = nullptr;
        for(unsigned i = 0; i < _linuxDevices.size(); i++)
            if(_linuxDevices[i].mac == mac)
            {
                dev = &_devices[i];
                lDev = &_linuxDevices[i];
            }

        //----- Check device -----//
        // Check if the device is known
        if(!dev)
        {
            LOG_WARN(_debugName.getString(), "Can't connect to unknown device [*y]yet[]: [w]$0", MACToString(mac));
            sd_bus_error_free(&error);
            return false;
        }
        dev->connected = false;
        lDev->servicesResolved = false;

        // Create device path based on MAC address
        lDev->path = _adapter + "/dev_XX_XX_XX_XX_XX_XX";
        r = snprintf(lDev->path.data(), lDev->path.size()+1,
                "%s/dev_%02X_%02X_%02X_%02X_%02X_%02X", _adapter.c_str(), mac[5],
                mac[4], mac[3], mac[2], mac[1], mac[0]);

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to contruct device path for [w]$0[]", MACToString(mac));
            sd_bus_error_free(&error);
            return false;
        }

        //----- Get connection status -----//
        // Check if it already is connected
        // This also serves as a mean to check if the object path is known in DBus
        int connected = -1;
        r = sd_bus_get_property_trivial(_bus, "org.bluez", lDev->path.c_str(),
                "org.bluez.Device1", "Connected", &error,
                'b', &connected);
        if(connected == 1)
            connStatus = CONNECTED;
        else if(connected == 0)
            connStatus = KNOWN_DEVICE_NOT_CONNECTED;
        if(r < 0)
        {
            if(sd_bus_error_has_name(&error, SD_BUS_ERROR_UNKNOWN_OBJECT))
            {
                // Device is unknown, mark for ConnectDevice API below
                connStatus = UNKNOWN_DEVICE_NOT_CONNECTED;
            }
            else
            {
                LOG_ERROR(_debugName.getString(), "Failed to get connect: [w]$0", error.message);
                sd_bus_error_free(&error);
                return false;
            }
        }

        //----- Check connection status -----//
        if(connStatus == CONNECTED)
        {
            LOG_INFO(_debugName.getString(), "Device [w]$0[] already was connected", MACToString(mac));

            // Get ServicesResolved status
            int sr;
            r = sd_bus_get_property_trivial(_bus, "org.bluez", lDev->path.c_str(),
                    "org.bluez.Device1", "ServicesResolved",
                    &error, 'b', &sr);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Failed to get ServicesResolved");
                disconnect(mac);
                sd_bus_error_free(&error);
                return false;
            }
            lDev->servicesResolved = sr;
        }
        else if(connStatus == UNDEFINED)
        {
            LOG_ERROR(_debugName.getString(), "Invalid connection status");
            sd_bus_error_free(&error);
            return false;
        }

        //----- Register connect signal -----//
        // Connect signal for device properties changed 
        r = sd_bus_match_signal(_bus, &lDev->connectSlot, "org.bluez",
                lDev->path.c_str(), "org.freedesktop.DBus.Properties",
                "PropertiesChanged", &LinuxBluetooth::bluezConnectCallback, allocPayload(mac));
        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Failed to add connect signal");
            sd_bus_error_free(&error);
            return false;
        }

        //----- Connect to the device -----//
        //  - If it is known, connect by the normal Connect API
        //  - If it is unknown, try using the new (Bluez 5.49) ConnectDevice API 
        //    for unknown (not yet discovered) devices
        if(connStatus == KNOWN_DEVICE_NOT_CONNECTED)
        {
            LOG_INFO(_debugName.getString(), "[*y](Connect)[y] Connect known");
            if(!bluezConnectKnown(lDev))
                return false;
        }
        else if(connStatus == UNKNOWN_DEVICE_NOT_CONNECTED)
        {
            LOG_INFO(_debugName.getString(), "[*y](Connect)[y] Connect unknown");
            if(!bluezConnectUnknown(lDev))
                return false;
        }

        // TODO Change status to connected if lDev->servicesResolved was set to true
        // TODO Disconnect if timeout while connecting
        return true;
    }

    int LinuxBluetooth::bluezConnectKnownCallback(sd_bus_message* reply, void* user, sd_bus_error* err)
    {
        LOG_INFO("io::LinuxBluetooth", "Connect known callback");
        int r = 0;

        //----- Unpack payload -----//
        CallbackPayload* cPayload = static_cast<CallbackPayload*>(user);
        LinuxBluetooth* linuxBluetooth = nullptr;
        LinuxDevice* lDev = nullptr;
        Device* dev = nullptr;
        if(unpackCallbackPayload(cPayload, &linuxBluetooth, &lDev, &dev) < 0)
            return -1;

        //----- Read error message -----//
        const sd_bus_error* error = sd_bus_message_get_error(reply);
        if(error != NULL)
        {
            r = -sd_bus_message_get_errno(reply);
            LOG_ERROR("io::LinuxBluetooth", "Connect known error: $0 '$1' ($2)", error->name, error->message, r);
            return r;
        }

        //----- Success -----//
        dev->connected = true;
        LOG_SUCCESS("io::LinuxBluetooth", "Connected to [w]$0[]", MACToString(lDev->mac));

        return r;
    }

    bool LinuxBluetooth::bluezConnectKnown(LinuxDevice* lDev)
    {
        int r;
        sd_bus_message* call = NULL;

        r = sd_bus_message_new_method_call(_bus, &call, "org.bluez",
                lDev->path.c_str(), "org.bluez.Device1",
                "Connect");

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Connect known failed to create message");
            sd_bus_message_unref(call);
            return r;
        }

        lDev->connectAsyncDone = false;

        // Call connect async
        r = sd_bus_call_async(_bus, NULL, call, 
                &LinuxBluetooth::bluezConnectKnownCallback, allocPayload(lDev->mac), 10 * 1000000);// timeout 10 seconds

        if(r < 0)
        {
            LOG_ERROR(_debugName.getString(), "Connect known failed to call async connect");
            sd_bus_message_unref(call);
            return r;
        }

        sd_bus_message_unref(call);
        return r;
    }

    int LinuxBluetooth::bluezConnectUnknownCallback(sd_bus_message* reply, void* user, sd_bus_error* err)
    {
        LOG_INFO("io::LinuxBluetooth", "Connect unknown callback [*y](not tested yet)[]");
        int r;
        char* opath;

        //----- Unpack payload -----//
        CallbackPayload* cPayload = static_cast<CallbackPayload*>(user);
        LinuxBluetooth* linuxBluetooth = nullptr;
        LinuxDevice* lDev = nullptr;
        Device* dev = nullptr;
        if(unpackCallbackPayload(cPayload, &linuxBluetooth, &lDev, &dev) < 0)
            return -1;

        lDev->connectAsyncDone = true;

        //----- Read error message -----//
        const sd_bus_error* error = sd_bus_message_get_error(reply);
        if(error != NULL)
        {
            if(sd_bus_error_has_name(err, SD_BUS_ERROR_UNKNOWN_METHOD))
            {
                LOG_ERROR("io::LinuxBluetooth", "Connect unknown failed: Bluez < 5.49 doesn't support ConnectDevice");
                r = -2;
            }
            else
            {
                r = -sd_bus_message_get_errno(reply);
                LOG_ERROR("io::LinuxBluetooth", "Connect unknown error: $0 '$1' ($2)", error->name, error->message, r);
            }
            return r;
        }

        //----- Read connect message -----//
        r = sd_bus_message_read_basic(reply, 'o', &opath);
        if(r < 0)
        {
            LOG_ERROR("io::LinuxBluetooth", "Connect unknown invalid reply");
            return r;
        }

        if(strcmp(opath, lDev->path.c_str()) != 0)
        {
            LOG_ERROR("io::LinuxBluetooth", "Connect unknown device paths don't match [w]$0[] and [w]$1[]", opath, lDev->path);
            return -1;
        }

        dev->connected = true;
        LOG_SUCCESS("io::LinuxBluetooth", "Connected to [w]$0[]", MACToString(lDev->mac));

        return r;
    }

    bool LinuxBluetooth::bluezConnectUnknown(LinuxDevice* lDev)
    {
        int r;
        sd_bus_message* call = NULL;

        // Try to connect with random/public address
        for(std::string addrType : { "random", "public" })
        {
            //----- Create message -----//
            r = sd_bus_message_new_method_call(_bus, &call, "org.bluez",
                    lDev->path.c_str(), "org.bluez.Adapter1",
                    "ConnectDevice");
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Connect unknown failed to create message");
                sd_bus_message_unref(call);
                continue;
            }

            //----- Populate message -----//
            // Open array
            r = sd_bus_message_open_container(call, 'a', "{sv}");
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Connect unknown failed to open message container");
                sd_bus_message_unref(call);
                continue;
            }

            r = bluezAppendProperty(call, "Address", 's', MACToString(lDev->mac).c_str());
            if(r < 0)
            {
                sd_bus_message_unref(call);
                continue;
            }

            // Set AddressType to public/random to connect 
            // with BLE (otherwise will attempt (BR/EDR))
            r = bluezAppendProperty(call, "AddressType", 's', addrType.c_str());
            if(r < 0)
            {
                sd_bus_message_unref(call);
                continue;
            }

            // Close array
            r = sd_bus_message_close_container(call);
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Connect unknown failed to close message container");
                sd_bus_message_unref(call);
                continue;
            }

            //----- Send connect call async -----//
            lDev->connectAsyncDone = false;

            // Call ConnectDevice (only supported from Bluez 5.49 on)
            r = sd_bus_call_async(_bus, NULL, call, &LinuxBluetooth::bluezConnectUnknownCallback, allocPayload(lDev->mac), 10 * 1000000);// 10 seconds
            if(r < 0)
            {
                LOG_ERROR(_debugName.getString(), "Connect unknown failed to call async");
                sd_bus_message_unref(call);
                continue;
            }

            LOG_INFO(_debugName.getString(), "Trying to connect to [w]$0[] with [w]$1[] address", MACToString(lDev->mac), addrType);
            // If no error, finish
            break;
        }
        sd_bus_message_unref(call);
        return true;
    }

    bool LinuxBluetooth::disconnect(std::array<uint8_t, 6> mac)
    {
        return true;
    }

    //----------------------------------------//
    //---------- Callback Payloads -----------//
    //----------------------------------------//
    int LinuxBluetooth::unpackCallbackPayload(CallbackPayload* cPayload, LinuxBluetooth** linuxBluetooth, LinuxDevice** lDev, Device** dev)
    {
        if(cPayload == nullptr || (cPayload != nullptr && cPayload->linuxBluetooth == nullptr))
        {
            LOG_ERROR("io::LinuxBluetooth", "(unpackCallbackPayload) Undefined payload");
            return -1;
        }
        *linuxBluetooth = cPayload->linuxBluetooth;

        // Get devices
        bool found = false;
        for(unsigned i = 0; i < (*linuxBluetooth)->_devices.size(); i++)
            if((*linuxBluetooth)->_devices[i].mac == cPayload->mac)
            {
                *lDev = &(*linuxBluetooth)->_linuxDevices[i];
                *dev = &(*linuxBluetooth)->_devices[i];
                found = true;
                break;
            }

        if(!found)
        {
            LOG_ERROR("io::LinuxBluetooth", "(unpackCallbackPayload) Unable to find device");
            return -1;
        }

        // Free payload
        (*linuxBluetooth)->freePayload(cPayload);

        return 0;
    }

    LinuxBluetooth::CallbackPayload* LinuxBluetooth::allocPayload(std::array<uint8_t, 6> mac)
    {
        for(auto& cPayload : _callbackPayloads)
            if(cPayload.linuxBluetooth == nullptr)
            {
                cPayload.linuxBluetooth = this;
                cPayload.mac = mac;
                return &cPayload;
            }
        return nullptr;
    }

    {
    void LinuxBluetooth::freePayload(CallbackPayload* cPayload)
        cPayload->linuxBluetooth = nullptr;
        std::fill(std::begin(cPayload->mac), std::end(cPayload->mac), 0);
    }
}
#include <atta/ioSystem/bluetooth/linuxBluetoothMsg.cpp>
#endif//ATTA_OS_LINUX && ATTA_SYSTEMD_SUPPORT 
