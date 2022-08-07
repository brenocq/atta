//--------------------------------------------------
// Atta IO Module
// linuxBluetooth.cpp
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#if defined(ATTA_OS_LINUX) && defined(ATTA_SYSTEMD_SUPPORT)
#include <atta/io/bluetooth/linuxBluetooth.h>
// Thanks to https://github.com/infsoft-locaware/blzlib

namespace atta::io {
LinuxBluetooth::LinuxBluetooth(Bluetooth::CreateInfo info) : Bluetooth(info), _bus(nullptr), _scanSlot(nullptr) {
    // Clear callback payload
    for (auto& cPayload : _callbackPayloads) {
        cPayload.linuxBluetooth = nullptr;
        std::fill(std::begin(cPayload.mac), std::end(cPayload.mac), 0);
    }
}

LinuxBluetooth::~LinuxBluetooth() { stop(); }

bool LinuxBluetooth::start() {
    if (_bus && !stop()) {
        LOG_ERROR(_debugName.getString(), "Could not stop previous system bus to start again");
        return false;
    }

    _adapter = "/org/bluez/hci0";

    int r;
    sd_bus_error error = SD_BUS_ERROR_NULL;

    // Connect to the system bus
    r = sd_bus_default_system(&_bus);
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Failed to connect to system bus: [w]$0", strerror(-r));
        return false;
    }
    LOG_VERBOSE(_debugName.getString(), "Connected to system bus");

    // Power on if necessary
    r = sd_bus_set_property(_bus, "org.bluez", _adapter.c_str(), "org.bluez.Adapter1", "Powered", &error, "b", 1);
    if (r < 0) {
        if (sd_bus_error_has_name(&error, SD_BUS_ERROR_UNKNOWN_OBJECT))
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

bool LinuxBluetooth::stop() {
    if (_scanSlot)
        stopScan();
    if (_bus)
        sd_bus_unref(_bus);
    _devices.clear();
    return true;
}

bool LinuxBluetooth::update() {
    if (!_bus) {
        LOG_WARN(_debugName.getString(), "Bluetooth must be started before updating");
        return false;
    }

    int r = sd_bus_process(_bus, NULL);
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Update failed to process system bus: [w]$0", strerror(-r));
        return false;
    }

    r = sd_bus_wait(_bus, 100); // Wait 0.1 ms
    if (r < 0 && -r != EINTR) {
        LOG_ERROR(_debugName.getString(), "Update failed to wait: [w]$0", strerror(-r));
        return false;
    }

    return true;
}

bool LinuxBluetooth::populateKnownDevices() {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message* reply = NULL;
    int r;

    r = sd_bus_call_method(_bus, "org.bluez", "/", "org.freedesktop.DBus.ObjectManager", "GetManagedObjects", &error, &reply, "");

    if (r < 0) {
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
int LinuxBluetooth::bluezScanCallback(sd_bus_message* m, void* user, sd_bus_error* err) {
    LinuxBluetooth* linuxBluetooth = static_cast<LinuxBluetooth*>(user);
    if (linuxBluetooth == nullptr) {
        LOG_ERROR("io::LinuxBluetooth", "Scan callback to unknown instance");
        return -1;
    }

    const char* opath;

    // Read object path
    int r = sd_bus_message_read_basic(m, 'o', &opath);
    if (r < 0) {
        LOG_ERROR("io::LinuxBluetooth", "Failed to read bus message");
        return r;
    }

    // Check if the message is to this adapter
    std::string adapter = linuxBluetooth->getAdapter();
    if (strncmp(opath, adapter.c_str(), adapter.size()) == 0) {
        // Parse message
        return linuxBluetooth->bluezParseInterfaces(m, opath, MSG_DEVICE_SCAN);
    } else {
        // Skip message
        r = sd_bus_message_skip(m, "a{sa{sv}}");
        if (r < 0) {
            LOG_ERROR("io::LinuxBluetooth", "Failed to skip bus message");
            return r;
        }
        LOG_VERBOSE("io::LinuxBluetooth", "Skip message to $0", opath);
    }

    return 0;
}

bool LinuxBluetooth::startScan() {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    int r;

    r = sd_bus_match_signal(_bus, &_scanSlot, "org.bluez", "/", "org.freedesktop.DBus.ObjectManager", "InterfacesAdded",
                            &atta::io::LinuxBluetooth::bluezScanCallback, this);

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Failed to add bluez interface");
        return false;
    }

    r = sd_bus_call_method(_bus, "org.bluez", _adapter.c_str(), "org.bluez.Adapter1", "StartDiscovery", &error, NULL, "");

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Failed to start scan: [w]$0", error.message);
        sd_bus_error_free(&error);
        return false;
    }

    LOG_VERBOSE(_debugName.getString(), "Started scanning...");
    sd_bus_error_free(&error);
    return true;
}

bool LinuxBluetooth::stopScan() {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    int r;

    r = sd_bus_call_method(_bus, "org.bluez", _adapter.c_str(), "org.bluez.Adapter1", "StopDiscovery", &error, NULL, "");

    if (r < 0) {
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
int LinuxBluetooth::bluezDeviceCallback(sd_bus_message* m, void* user, sd_bus_error* err) {
    //----- Unpack payload -----//
    CallbackPayload* cPayload = static_cast<CallbackPayload*>(user);
    LinuxBluetooth* linuxBluetooth = nullptr;
    LinuxDevice* lDev = nullptr;
    Device* dev = nullptr;
    // Because this callback will be called multiple times, we will not free the CallbackPayload
    if (unpackCallbackPayload(cPayload, &linuxBluetooth, &lDev, &dev, false) < 0)
        return -1;

    //----- Success -----//
    linuxBluetooth->bluezParseInterface(m, nullptr, MSG_DEVICE, lDev);
    LOG_INFO("io::LinuxBluetooth", "Device [w]$0[] properties changed", MACToString(lDev->mac));
    linuxBluetooth->populateDeviceServices(lDev);

    return 0;
}

bool LinuxBluetooth::connect(std::array<uint8_t, 6> mac) {
    int r;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    enum ConnStatus { UNDEFINED = 0, CONNECTED, KNOWN_DEVICE_NOT_CONNECTED, UNKNOWN_DEVICE_NOT_CONNECTED };
    ConnStatus connStatus = UNDEFINED;

    Device* dev = nullptr;
    LinuxDevice* lDev = nullptr;
    for (unsigned i = 0; i < _linuxDevices.size(); i++)
        if (_linuxDevices[i].mac == mac) {
            dev = &_devices[i];
            lDev = &_linuxDevices[i];
        }

    //----- Check device -----//
    // Check if the device is known
    if (!dev) {
        LOG_WARN(_debugName.getString(), "Can't connect to unknown device [*y]yet[]: [w]$0", MACToString(mac));
        sd_bus_error_free(&error);
        return false;
    }
    dev->connected = false;
    lDev->servicesResolved = false;

    //----- Get connection status -----//
    // Check if it already is connected
    // This also serves as a mean to check if the object path is known in DBus
    int connected = -1;
    r = sd_bus_get_property_trivial(_bus, "org.bluez", lDev->path.c_str(), "org.bluez.Device1", "Connected", &error, 'b', &connected);
    if (connected == 1)
        connStatus = CONNECTED;
    else if (connected == 0)
        connStatus = KNOWN_DEVICE_NOT_CONNECTED;
    if (r < 0) {
        if (sd_bus_error_has_name(&error, SD_BUS_ERROR_UNKNOWN_OBJECT)) {
            // Device is unknown, mark for ConnectDevice API below
            connStatus = UNKNOWN_DEVICE_NOT_CONNECTED;
        } else {
            LOG_ERROR(_debugName.getString(), "Failed to get connect: [w]$0", error.message);
            sd_bus_error_free(&error);
            return false;
        }
    }

    //----- Check connection status -----//
    if (connStatus == CONNECTED) {
        LOG_INFO(_debugName.getString(), "Device [w]$0[] already was connected", MACToString(mac));

        // Get ServicesResolved status
        int sr;
        r = sd_bus_get_property_trivial(_bus, "org.bluez", lDev->path.c_str(), "org.bluez.Device1", "ServicesResolved", &error, 'b', &sr);
        if (r < 0) {
            LOG_ERROR(_debugName.getString(), "Failed to get ServicesResolved");
            disconnect(mac);
            sd_bus_error_free(&error);
            return false;
        }
        lDev->servicesResolved = sr;

        // Populate services
        populateDeviceServices(lDev);
    } else if (connStatus == UNDEFINED) {
        LOG_ERROR(_debugName.getString(), "Invalid connection status");
        sd_bus_error_free(&error);
        return false;
    }

    //----- Register connect signal -----//
    // Connect signal for device properties changed
    r = sd_bus_match_signal(_bus, &lDev->signalSlot, "org.bluez", lDev->path.c_str(), "org.freedesktop.DBus.Properties", "PropertiesChanged",
                            &LinuxBluetooth::bluezDeviceCallback, allocPayload(mac));
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Failed to add connect signal");
        sd_bus_error_free(&error);
        return false;
    }

    //----- Connect to the device -----//
    //  - If it is known, connect by the normal Connect API
    //  - If it is unknown, try using the new (Bluez 5.49) ConnectDevice API
    //    for unknown (not yet discovered) devices
    if (connStatus == KNOWN_DEVICE_NOT_CONNECTED) {
        LOG_INFO(_debugName.getString(), "[*y](Connect)[y] Connect known");
        if (!bluezConnectKnown(lDev))
            return false;
    } else if (connStatus == UNKNOWN_DEVICE_NOT_CONNECTED) {
        LOG_INFO(_debugName.getString(), "[*y](Connect)[y] Connect unknown");
        if (!bluezConnectUnknown(lDev))
            return false;
    }

    // TODO Change status to connected if lDev->servicesResolved was set to true
    // TODO Disconnect if timeout while connecting
    return true;
}

int LinuxBluetooth::bluezConnectKnownCallback(sd_bus_message* reply, void* user, sd_bus_error* err) {
    int r = 0;

    //----- Unpack payload -----//
    CallbackPayload* cPayload = static_cast<CallbackPayload*>(user);
    LinuxBluetooth* linuxBluetooth = nullptr;
    LinuxDevice* lDev = nullptr;
    Device* dev = nullptr;
    if (unpackCallbackPayload(cPayload, &linuxBluetooth, &lDev, &dev) < 0)
        return -1;

    //----- Read error message -----//
    const sd_bus_error* error = sd_bus_message_get_error(reply);
    if (error != NULL) {
        r = -sd_bus_message_get_errno(reply);
        LOG_ERROR("io::LinuxBluetooth", "Connect known error: $0 '$1' ($2)", error->name, error->message, r);
        return r;
    }

    //----- Success -----//
    dev->connected = true;
    LOG_SUCCESS("io::LinuxBluetooth", "Connected to [w]$0[]", MACToString(lDev->mac));
    linuxBluetooth->populateDeviceServices(lDev);

    return r;
}

bool LinuxBluetooth::bluezConnectKnown(LinuxDevice* lDev) {
    int r;
    sd_bus_message* call = NULL;

    r = sd_bus_message_new_method_call(_bus, &call, "org.bluez", lDev->path.c_str(), "org.bluez.Device1", "Connect");

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Connect known failed to create message");
        sd_bus_message_unref(call);
        return r;
    }

    lDev->connectAsyncDone = false;

    // Call connect async
    r = sd_bus_call_async(_bus, NULL, call, &LinuxBluetooth::bluezConnectKnownCallback, allocPayload(lDev->mac), 10 * 1000000); // timeout 10 seconds

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Connect known failed to call async connect");
        sd_bus_message_unref(call);
        return r;
    }

    sd_bus_message_unref(call);
    return r;
}

int LinuxBluetooth::bluezConnectUnknownCallback(sd_bus_message* reply, void* user, sd_bus_error* err) {
    LOG_INFO("io::LinuxBluetooth", "Connect unknown callback [*y](not tested yet)[]");
    int r;
    char* opath;

    //----- Unpack payload -----//
    CallbackPayload* cPayload = static_cast<CallbackPayload*>(user);
    LinuxBluetooth* linuxBluetooth = nullptr;
    LinuxDevice* lDev = nullptr;
    Device* dev = nullptr;
    if (unpackCallbackPayload(cPayload, &linuxBluetooth, &lDev, &dev) < 0)
        return -1;

    lDev->connectAsyncDone = true;

    //----- Read error message -----//
    const sd_bus_error* error = sd_bus_message_get_error(reply);
    if (error != NULL) {
        if (sd_bus_error_has_name(err, SD_BUS_ERROR_UNKNOWN_METHOD)) {
            LOG_ERROR("io::LinuxBluetooth", "Connect unknown failed: Bluez < 5.49 doesn't support ConnectDevice");
            r = -2;
        } else {
            r = -sd_bus_message_get_errno(reply);
            LOG_ERROR("io::LinuxBluetooth", "Connect unknown error: $0 '$1' ($2)", error->name, error->message, r);
        }
        return r;
    }

    //----- Read connect message -----//
    r = sd_bus_message_read_basic(reply, 'o', &opath);
    if (r < 0) {
        LOG_ERROR("io::LinuxBluetooth", "Connect unknown invalid reply");
        return r;
    }

    if (strcmp(opath, lDev->path.c_str()) != 0) {
        LOG_ERROR("io::LinuxBluetooth", "Connect unknown device paths don't match [w]$0[] and [w]$1[]", opath, lDev->path);
        return -1;
    }

    dev->connected = true;
    LOG_SUCCESS("io::LinuxBluetooth", "Connected to [w]$0[]", MACToString(lDev->mac));

    return r;
}

bool LinuxBluetooth::bluezConnectUnknown(LinuxDevice* lDev) {
    int r;
    sd_bus_message* call = NULL;

    // Try to connect with random/public address
    for (std::string addrType : {"random", "public"}) {
        //----- Create message -----//
        r = sd_bus_message_new_method_call(_bus, &call, "org.bluez", lDev->path.c_str(), "org.bluez.Adapter1", "ConnectDevice");
        if (r < 0) {
            LOG_ERROR(_debugName.getString(), "Connect unknown failed to create message");
            sd_bus_message_unref(call);
            continue;
        }

        //----- Populate message -----//
        // Open array
        r = sd_bus_message_open_container(call, 'a', "{sv}");
        if (r < 0) {
            LOG_ERROR(_debugName.getString(), "Connect unknown failed to open message container");
            sd_bus_message_unref(call);
            continue;
        }

        r = bluezAppendProperty(call, "Address", 's', MACToString(lDev->mac).c_str());
        if (r < 0) {
            sd_bus_message_unref(call);
            continue;
        }

        // Set AddressType to public/random to connect
        // with BLE (otherwise will attempt (BR/EDR))
        r = bluezAppendProperty(call, "AddressType", 's', addrType.c_str());
        if (r < 0) {
            sd_bus_message_unref(call);
            continue;
        }

        // Close array
        r = sd_bus_message_close_container(call);
        if (r < 0) {
            LOG_ERROR(_debugName.getString(), "Connect unknown failed to close message container");
            sd_bus_message_unref(call);
            continue;
        }

        //----- Send connect call async -----//
        lDev->connectAsyncDone = false;

        // Call ConnectDevice (only supported from Bluez 5.49 on)
        r = sd_bus_call_async(_bus, NULL, call, &LinuxBluetooth::bluezConnectUnknownCallback, allocPayload(lDev->mac), 10 * 1000000); // 10 seconds
        if (r < 0) {
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

bool LinuxBluetooth::disconnect(std::array<uint8_t, 6> mac) {
    LinuxDevice* lDev;
    Device* dev;
    for (unsigned i = 0; i < _devices.size(); i++)
        if (_devices[i].mac == mac) {
            lDev = &_linuxDevices[i];
            dev = &_devices[i];
        }

    if (!dev || !lDev)
        return false;

    if (lDev->signalSlot)
        lDev->signalSlot = sd_bus_slot_unref(lDev->signalSlot);

    if (dev->connected) {
        sd_bus_error error = SD_BUS_ERROR_NULL;
        int r;

        r = sd_bus_call_method(_bus, "org.bluez", lDev->path.c_str(), "org.bluez.Device1", "Disconnect", &error, NULL, "");

        if (r < 0) {
            LOG_ERROR(_debugName.getString(), "Failed to disconnect [w]$0[]: $1", MACToString(mac), error.message);
            sd_bus_error_free(&error);
            return r;
        }
        sd_bus_error_free(&error);
    }

    dev->connected = false;
    LOG_INFO(_debugName.getString(), "[w]$0[] disconnected", MACToString(mac));

    return true;
}

//----------------------------------------//
//------- Services/Characteristics -------//
//----------------------------------------//
bool LinuxBluetooth::populateDeviceServices(LinuxDevice* lDev) {
    LOG_INFO(_debugName.getString(), "[*w](populateDeviceServices) [w]$0[]", MACToString(lDev->mac));

    // Get device
    Device* dev = nullptr;
    for (unsigned i = 0; i < _devices.size(); i++)
        if (_devices[i].mac == lDev->mac) {
            dev = &_devices[i];
            break;
        }

    //---------- Solve services ----------//
    // Get services
    char** serviceUUIDs = nullptr;
    sd_bus_error error = SD_BUS_ERROR_NULL;
    int r = sd_bus_get_property_strv(_bus, "org.bluez", lDev->path.c_str(), "org.bluez.Device1", "UUIDs", &error, &serviceUUIDs);

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Failed to get [w]$0[] services: $1", MACToString(lDev->mac), error.message);
        sd_bus_error_free(&error);
        return false;
    }
    sd_bus_error_free(&error);

    // Populate services
    dev->services.clear();
    lDev->services.clear();
    if (serviceUUIDs != nullptr)
        for (int i = 0; serviceUUIDs[i] != nullptr; i++) {
            Service serv{};
            LinuxService lServ{};
            lServ.uuid = serv.uuid = std::string(serviceUUIDs[i]);
            dev->services.push_back(serv);
            lDev->services.push_back(lServ);
            LOG_INFO(_debugName.getString(), "(populateDeviceServices) [w]$0[] service $1", MACToString(lDev->mac), serv.uuid);
        }

    //---------- Solve characteristics ----------//
    for (LinuxService& lServ : lDev->services)
        populateServiceChars(lDev, &lServ);

    return true;
}

bool LinuxBluetooth::populateServiceChars(LinuxDevice* lDev, LinuxService* lServ) {
    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message* reply = NULL;
    int r;

    r = sd_bus_call_method(_bus, "org.bluez", "/", "org.freedesktop.DBus.ObjectManager", "GetManagedObjects", &error, &reply, "");

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "(populateServiceChars) Failed to get managed objects: $0", error.message);
        sd_bus_error_free(&error);
        sd_bus_message_unref(reply);
        return false;
    }

    // Find linux service path
    r = bluezParseObjects(reply, lDev->path.c_str(), MSG_SERVICE, lServ);

    // Parse all characteristics data
    if (lServ->path != "") {
        sd_bus_message_rewind(reply, true);
        r = bluezParseObjects(reply, lServ->path.c_str(), MSG_CHARS, lServ);
    }

    sd_bus_error_free(&error);
    sd_bus_message_unref(reply);
    return r >= 0;
}

//----------------------------------------//
//---------- Read/Write/Notify -----------//
//----------------------------------------//
//---------- Read -----------//
bool LinuxBluetooth::readChar(const Char& ch, uint8_t* data, size_t* len) {
    LinuxChar* lch = nullptr;
    for (auto& lDev : _linuxDevices)
        for (auto& lServ : lDev.services)
            for (auto& llch : lServ.chars)
                if (llch.uuid == ch.uuid)
                    lch = &llch;
    if (lch == nullptr) {
        LOG_ERROR(_debugName.getString(), "Could not find registered characteristic to read");
        return false;
    }

    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message* reply = NULL;
    const void* ptr;
    size_t rlen = -1;
    int r;

    if (!bool(ch.flags & (CharFlags::READ))) {
        LOG_ERROR(_debugName.getString(), "Characteristic [w]$0[] does not support read", ch.uuid);
        return false;
    }

    r = sd_bus_call_method(_bus, "org.bluez", lch->path.c_str(), "org.bluez.GattCharacteristic1", "ReadValue", &error, &reply, "a{sv}", 0);

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Read char failed: $0", error.message);
        sd_bus_error_free(&error);
        sd_bus_message_unref(reply);
        return false;
    }

    r = sd_bus_message_read_array(reply, 'y', &ptr, &rlen);
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Read char failed to read result: $0", error.message);
        sd_bus_error_free(&error);
        sd_bus_message_unref(reply);
        return false;
    }

    if (rlen > 0)
        memcpy(data, ptr, rlen < *len ? rlen : *len);

    *len = rlen;

    return r >= 0;
}

//---------- Write -----------//
bool LinuxBluetooth::writeChar(const Char& ch, const uint8_t* data, size_t len) {
    LinuxChar* lch = nullptr;
    for (auto& lDev : _linuxDevices)
        for (auto& lServ : lDev.services)
            for (auto& llch : lServ.chars)
                if (llch.uuid == ch.uuid)
                    lch = &llch;
    if (lch == nullptr) {
        LOG_ERROR(_debugName.getString(), "Could not find registered characteristic to write");
        return false;
    }

    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message* call = NULL;
    sd_bus_message* reply = NULL;
    int r;

    if (!bool(ch.flags & (CharFlags::ANY_WRITE))) {
        LOG_ERROR(_debugName.getString(), "Characteristic [w]$0[] does not support write", ch.uuid);
        return false;
    }

    r = sd_bus_message_new_method_call(_bus, &call, "org.bluez", lch->path.c_str(), "org.bluez.GattCharacteristic1", "WriteValue");

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Write char failed (create message)");
        sd_bus_error_free(&error);
        sd_bus_message_unref(call);
        sd_bus_message_unref(reply);
        return false;
    }

    r = sd_bus_message_append_array(call, 'y', data, len);
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Write char failed (append array)");
        sd_bus_error_free(&error);
        sd_bus_message_unref(call);
        sd_bus_message_unref(reply);
        return false;
    }

    r = sd_bus_message_open_container(call, 'a', "{sv}");
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Write char failed (open container)");
        sd_bus_error_free(&error);
        sd_bus_message_unref(call);
        sd_bus_message_unref(reply);
        return false;
    }

    r = sd_bus_message_close_container(call);
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Write char failed (close container)");
        sd_bus_error_free(&error);
        sd_bus_message_unref(call);
        sd_bus_message_unref(reply);
        return false;
    }

    r = sd_bus_call(_bus, call, 0, &error, &reply);
    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "Write char failed to write: $0", error.message);
        sd_bus_error_free(&error);
        sd_bus_message_unref(call);
        sd_bus_message_unref(reply);
        return false;
    }

    return r >= 0;
}

//---------- Notify -----------//
int LinuxBluetooth::bluezNotifyCallback(sd_bus_message* m, void* user, sd_bus_error* err) {
    int r;
    const void* ptr = nullptr;
    size_t len;

    //----- Unpack payload -----//
    NotifyCallbackPayload* cPayload = static_cast<NotifyCallbackPayload*>(user);
    LinuxBluetooth* linuxBluetooth = nullptr;
    LinuxChar* lch = nullptr;
    Char* ch = nullptr;
    if (unpackCallbackPayload(cPayload, &linuxBluetooth, &lch, &ch, false) < 0)
        return -1;

    //----- Check error -----//
    if (ch == nullptr || lch == nullptr || ch->notifyFunction == nullptr) {
        LOG_ERROR("io::LinuxBluetooth", "Characteristic [w]$0[] has no notify function set", ch->uuid);
        return -1;
    }

    r = linuxBluetooth->bluezParseNotify(m, ch, lch, &ptr, &len);

    if (r > 0 && ptr != nullptr && ch->notifyFunction)
        ch->notifyFunction(*ch, (uint8_t*)ptr, len);

    return 0;
}

bool LinuxBluetooth::notifyCharStart(const Char& ch, NotifyFunction func) {
    Char* cch = nullptr;
    LinuxChar* lch = nullptr;
    LinuxDevice* lDev = nullptr;
    for (auto& llDev : _linuxDevices)
        for (auto& lServ : llDev.services)
            for (auto& llch : lServ.chars)
                if (llch.uuid == ch.uuid) {
                    lch = &llch;
                    lDev = &llDev;
                }
    for (auto& llDev : _devices)
        for (auto& lServ : llDev.services)
            for (auto& llch : lServ.chars)
                if (llch.uuid == ch.uuid) {
                    cch = &llch;
                }
    if (lch == nullptr) {
        LOG_ERROR(_debugName.getString(), "Could not find registered characteristic to start notify");
        return false;
    }

    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message* reply = NULL;
    int r;

    if (!bool(ch.flags & (CharFlags::NOTIFY | CharFlags::INDICATE))) {
        LOG_ERROR(_debugName.getString(), "Characteristic [w]$0[] does not support notify", ch.uuid);
        return false;
    }
    cch->notifyFunction = func;

    r = sd_bus_match_signal(_bus, &lch->notifySlot, "org.bluez", lch->path.c_str(), "org.freedesktop.DBus.Properties", "PropertiesChanged",
                            &LinuxBluetooth::bluezNotifyCallback, allocPayload(lDev->mac, lch->uuid));

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "(notifyCharStart) Failed to register notify");
        sd_bus_error_free(&error);
        sd_bus_message_unref(reply);
        return -1;
    }

    r = sd_bus_call_method(_bus, "org.bluez", lch->path.c_str(), "org.bluez.GattCharacteristic1", "StartNotify", &error, &reply, "");

    if (r < 0) {
        LOG_ERROR(_debugName.getString(), "(notifyCharStart) Failed to start notify: $0", error.message);
        sd_bus_error_free(&error);
        sd_bus_message_unref(reply);
        return -1;
    }

    return 0;
}

bool LinuxBluetooth::notifyCharStop(const Char& ch) {
    Char* cch = nullptr;
    LinuxChar* lch = nullptr;
    LinuxDevice* lDev = nullptr;
    for (auto& llDev : _linuxDevices)
        for (auto& lServ : llDev.services)
            for (auto& llch : lServ.chars)
                if (llch.uuid == ch.uuid) {
                    lch = &llch;
                    lDev = &llDev;
                }
    for (auto& llDev : _devices)
        for (auto& lServ : llDev.services)
            for (auto& llch : lServ.chars)
                if (llch.uuid == ch.uuid) {
                    cch = &llch;
                }

    sd_bus_error error = SD_BUS_ERROR_NULL;
    sd_bus_message* reply = NULL;
    int r;

    if (cch == nullptr || lch == nullptr || lch->notifySlot == nullptr) {
        LOG_ERROR(_debugName.getString(), "(notifyCharStop) Failed to find registered characteristic");
        return false;
    }

    r = sd_bus_call_method(_bus, "org.bluez", lch->path.c_str(), "org.bluez.GattCharacteristic1", "StopNotify", &error, &reply, "");

    if (r < 0)
        LOG_ERROR(_debugName.getString(), "(notifyCharStop) Failed to stop notify: $0", error.message);

    lch->notifySlot = sd_bus_slot_unref(lch->notifySlot);
    cch->notifyFunction = nullptr;
    cch->notifying = false;

    sd_bus_error_free(&error);
    sd_bus_message_unref(reply);
    return r >= 0;
}

//----------------------------------------//
//---------- Callback Payloads -----------//
//----------------------------------------//
int LinuxBluetooth::unpackCallbackPayload(CallbackPayload* cPayload, LinuxBluetooth** linuxBluetooth, LinuxDevice** lDev, Device** dev,
                                          bool release) {
    if (cPayload == nullptr || (cPayload != nullptr && cPayload->linuxBluetooth == nullptr)) {
        LOG_ERROR("io::LinuxBluetooth", "(unpackCallbackPayload) Undefined payload");
        return -1;
    }
    *linuxBluetooth = cPayload->linuxBluetooth;

    // Get devices
    bool found = false;
    for (unsigned i = 0; i < (*linuxBluetooth)->_devices.size(); i++)
        if ((*linuxBluetooth)->_devices[i].mac == cPayload->mac) {
            *lDev = &(*linuxBluetooth)->_linuxDevices[i];
            *dev = &(*linuxBluetooth)->_devices[i];
            found = true;
            break;
        }

    if (!found) {
        LOG_ERROR("io::LinuxBluetooth", "(unpackCallbackPayload) Unable to find device");
        return -1;
    }

    // Free payload
    if (release)
        (*linuxBluetooth)->freePayload(cPayload);

    return 0;
}

int LinuxBluetooth::unpackCallbackPayload(NotifyCallbackPayload* cPayload, LinuxBluetooth** linuxBluetooth, LinuxChar** lch, Char** ch,
                                          bool release) {
    if (cPayload == nullptr || (cPayload != nullptr && cPayload->linuxBluetooth == nullptr)) {
        LOG_ERROR("io::LinuxBluetooth", "(unpackCallbackPayload notify) Undefined notify payload");
        return -1;
    }
    *linuxBluetooth = cPayload->linuxBluetooth;

    // Get characteristic
    bool found = false;
    for (unsigned i = 0; i < (*linuxBluetooth)->_devices.size(); i++)
        if ((*linuxBluetooth)->_devices[i].mac == cPayload->mac) {
            for (unsigned s = 0; s < (*linuxBluetooth)->_devices[i].services.size(); s++) {
                for (unsigned c = 0; c < (*linuxBluetooth)->_devices[i].services[s].chars.size(); c++) {
                    if ((*linuxBluetooth)->_devices[i].services[s].chars[c].uuid == cPayload->uuid) {
                        *lch = &(*linuxBluetooth)->_linuxDevices[i].services[s].chars[c];
                        *ch = &(*linuxBluetooth)->_devices[i].services[s].chars[c];
                        found = true;
                        break;
                    }
                }
            }
            break;
        }

    if (!found) {
        LOG_ERROR("io::LinuxBluetooth", "(unpackCallbackPayload notify) Unable to find characteristic");
        return -1;
    }

    // Free payload
    if (release)
        (*linuxBluetooth)->freePayload(cPayload);

    return 0;
}

LinuxBluetooth::CallbackPayload* LinuxBluetooth::allocPayload(std::array<uint8_t, 6> mac) {
    for (auto& cPayload : _callbackPayloads)
        if (cPayload.linuxBluetooth == nullptr) {
            cPayload.linuxBluetooth = this;
            cPayload.mac = mac;
            return &cPayload;
        }
    return nullptr;
}

void LinuxBluetooth::freePayload(CallbackPayload* cPayload) {
    cPayload->linuxBluetooth = nullptr;
    std::fill(std::begin(cPayload->mac), std::end(cPayload->mac), 0);
}

LinuxBluetooth::NotifyCallbackPayload* LinuxBluetooth::allocPayload(std::array<uint8_t, 6> mac, std::string uuid) {
    for (auto& cPayload : _notifyCallbackPayloads)
        if (cPayload.linuxBluetooth == nullptr) {
            cPayload.linuxBluetooth = this;
            cPayload.mac = mac;
            cPayload.uuid = uuid;
            return &cPayload;
        }
    return nullptr;
}

void LinuxBluetooth::freePayload(NotifyCallbackPayload* cPayload) {
    cPayload->linuxBluetooth = nullptr;
    std::fill(std::begin(cPayload->mac), std::end(cPayload->mac), 0);
    cPayload->uuid = "";
}
} // namespace atta::io
#include <atta/io/bluetooth/linuxBluetoothMsg.cpp>
#endif // ATTA_OS_LINUX && ATTA_SYSTEMD_SUPPORT
