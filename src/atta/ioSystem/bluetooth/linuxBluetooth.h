//--------------------------------------------------
// Atta IO System
// linuxBluetooth.h
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_BLUETOOTH_LINUX_BLUETOOTH_H
#define ATTA_IO_SYSTEM_BLUETOOTH_LINUX_BLUETOOTH_H
#if defined(ATTA_OS_LINUX) && defined(ATTA_SYSTEMD_SUPPORT)
#include <atta/ioSystem/bluetooth/bluetooth.h>
#include <systemd/sd-bus.h>

namespace atta::io
{
    class LinuxBluetooth final : public Bluetooth
    {
    public:
        LinuxBluetooth(Bluetooth::CreateInfo info);
        ~LinuxBluetooth();

        bool start() override;
        bool stop() override;
        bool update() override;
        bool startScan() override;
        bool stopScan() override;
        bool connect(std::array<uint8_t, 6> mac) override;
        bool disconnect(std::array<uint8_t, 6> mac) override;

        struct CallbackPayload
        {
            LinuxBluetooth* linuxBluetooth = nullptr;
            std::array<uint8_t, 6> mac;
        };

        static int bluezScanCallback(sd_bus_message* m, void* user, sd_bus_error* err);
        static int bluezConnectCallback(sd_bus_message* m, void* user, sd_bus_error* err);
        static int bluezConnectKnownCallback(sd_bus_message* reply, void* user, sd_bus_error* err);
        static int bluezConnectUnknownCallback(sd_bus_message* reply, void* user, sd_bus_error* err);
        std::string getAdapter() const { return _adapter; }

    private:
        enum BluezMsgType
        {
            MSG_DEVICE_SCAN = 0,
            MSG_DEVICE
        };

        struct LinuxDevice
        {
            std::array<uint8_t, 6> mac;
            std::string path;
            sd_bus_slot* connectSlot = nullptr;
            char** serviceUUIDs = nullptr;
            bool connectAsyncDone = false;
            bool servicesResolved = false;
        };

        bool populateKnownDevices();
        bool bluezConnectKnown(LinuxDevice* lDev);
        bool bluezConnectUnknown(LinuxDevice* lDev);

        int bluezParseObjects(sd_bus_message* m, const char* adapter, BluezMsgType msgType);
        int bluezParseObject(sd_bus_message* m, const char* adapter, BluezMsgType msgType);
        int bluezParseInterfaces(sd_bus_message* m, const char* opath, BluezMsgType msgType);
        int bluezParseInterface(sd_bus_message* m, const char* opath, BluezMsgType msgType, void* user = nullptr);
        int bluezParseDevice1(sd_bus_message* m, const char* opath, Device* dev, LinuxDevice* devL);
        int bluezReadVariant(sd_bus_message* m, char* type, void* dest);
        int bluezReadVariant(sd_bus_message* m, char*** dest);
        int bluezAppendProperty(sd_bus_message* m, const char* name, char type, const void* value);

        static int unpackCallbackPayload(CallbackPayload* cPayload, LinuxBluetooth** linuxBluetooth, LinuxDevice** lDev, Device** dev);
        CallbackPayload* allocPayload(std::array<uint8_t, 6> mac);
        void freePayload(CallbackPayload* cPayload);

        std::string _adapter;
        sd_bus* _bus;
        sd_bus_slot* _scanSlot;

        std::vector<LinuxDevice> _linuxDevices;// 1 to 1 relationship with _devices vector
        std::array<CallbackPayload, 10> _callbackPayloads;// At most 10 callbacks at the same time
    };
}

#endif// ATTA_OS_LINUX && ATTA_SYSTEMD_SUPPORT 
#endif// ATTA_IO_SYSTEM_BLUETOOTH_LINUX_BLUETOOTH_H
