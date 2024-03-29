//--------------------------------------------------
// Atta IO Module
// linuxBluetooth.h
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_BLUETOOTH_LINUX_BLUETOOTH_H
#define ATTA_IO_BLUETOOTH_LINUX_BLUETOOTH_H
#if defined(ATTA_OS_LINUX) && defined(ATTA_SYSTEMD_SUPPORT)

#include <atta/io/bluetooth/bluetooth.h>
#include <systemd/sd-bus.h>

namespace atta::io {

class LinuxBluetooth final : public Bluetooth {
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

    bool readChar(const Char& ch, uint8_t* data, size_t* len) override;
    bool writeChar(const Char& ch, const uint8_t* data, size_t len) override;
    bool notifyCharStart(const Char& ch, NotifyFunction func) override;
    bool notifyCharStop(const Char& ch) override;

    struct CallbackPayload {
        LinuxBluetooth* linuxBluetooth = nullptr;
        std::array<uint8_t, 6> mac;
    };

    struct NotifyCallbackPayload {
        LinuxBluetooth* linuxBluetooth = nullptr;
        std::array<uint8_t, 6> mac; // Device MAC
        std::string uuid;           // Characteristic UUID
    };

    static int bluezScanCallback(sd_bus_message* m, void* user, sd_bus_error* err);
    static int bluezDeviceCallback(sd_bus_message* m, void* user, sd_bus_error* err);
    static int bluezConnectKnownCallback(sd_bus_message* reply, void* user, sd_bus_error* err);
    static int bluezConnectUnknownCallback(sd_bus_message* reply, void* user, sd_bus_error* err);
    static int bluezNotifyCallback(sd_bus_message* m, void* user, sd_bus_error* err);
    std::string getAdapter() const { return _adapter; }

  private:
    enum BluezMsgType { MSG_DEVICE_SCAN = 0, MSG_DEVICE, MSG_SERVICE, MSG_CHARS };

    struct LinuxChar {
        std::string uuid;
        std::string path;
        sd_bus_slot* notifySlot = nullptr;
    };

    struct LinuxService {
        std::string uuid;
        std::string path;
        std::vector<LinuxChar> chars;
    };

    struct LinuxDevice {
        std::array<uint8_t, 6> mac;
        std::string path;
        sd_bus_slot* signalSlot = nullptr;
        char** serviceUUIDs = nullptr;
        bool connectAsyncDone = false;
        bool servicesResolved = false;
        std::vector<LinuxService> services;
    };

    bool populateKnownDevices();
    bool bluezConnectKnown(LinuxDevice* lDev);
    bool bluezConnectUnknown(LinuxDevice* lDev);
    bool populateDeviceServices(LinuxDevice* lDev);
    bool populateServiceChars(LinuxDevice* lDev, LinuxService* lServ);

    int bluezParseObjects(sd_bus_message* m, const char* adapter, BluezMsgType msgType, void* user = nullptr);
    int bluezParseObject(sd_bus_message* m, const char* adapter, BluezMsgType msgType, void* user = nullptr);
    int bluezParseInterfaces(sd_bus_message* m, const char* opath, BluezMsgType msgType, void* user = nullptr);
    int bluezParseInterface(sd_bus_message* m, const char* opath, BluezMsgType msgType, void* user = nullptr);
    int bluezParseDevice1(sd_bus_message* m, const char* opath, Device* dev, LinuxDevice* devL);
    int bluezParseService1(sd_bus_message* m, const char* opath, LinuxService* lServ);
    int bluezParseCharacteristic1(sd_bus_message* m, const char* opath, Char* ch, LinuxChar* lch);
    int bluezParseNotify(sd_bus_message* m, Char* ch, LinuxChar* lch, const void** ptr, size_t* len);
    int bluezReadVariant(sd_bus_message* m, char* type, void* dest);
    int bluezReadVariant(sd_bus_message* m, char*** dest);
    int bluezAppendProperty(sd_bus_message* m, const char* name, char type, const void* value);

    static int unpackCallbackPayload(CallbackPayload* cPayload, LinuxBluetooth** linuxBluetooth, LinuxDevice** lDev, Device** dev,
                                     bool release = true);
    static int unpackCallbackPayload(NotifyCallbackPayload* cPayload, LinuxBluetooth** linuxBluetooth, LinuxChar** lch, Char** ch,
                                     bool release = true);
    CallbackPayload* allocPayload(std::array<uint8_t, 6> mac);
    void freePayload(CallbackPayload* cPayload);
    NotifyCallbackPayload* allocPayload(std::array<uint8_t, 6> mac, std::string uuid);
    void freePayload(NotifyCallbackPayload* cPayload);

    std::string _adapter;
    sd_bus* _bus;
    sd_bus_slot* _scanSlot;

    std::vector<LinuxDevice> _linuxDevices;                        // 1 to 1 relationship with _devices vector
    std::array<CallbackPayload, 50> _callbackPayloads;             // At most 50 callbacks at the same time
    std::array<NotifyCallbackPayload, 50> _notifyCallbackPayloads; // At most 50 notify callbacks at the same time
};

} // namespace atta::io

#endif // ATTA_OS_LINUX && ATTA_SYSTEMD_SUPPORT
#endif // ATTA_IO_BLUETOOTH_LINUX_BLUETOOTH_H
