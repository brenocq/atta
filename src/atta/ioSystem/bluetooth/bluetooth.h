//--------------------------------------------------
// Atta IO System
// bluetooth.h
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_BLUETOOTH_BLUETOOTH_H
#define ATTA_IO_SYSTEM_BLUETOOTH_BLUETOOTH_H
#include <atta/core/stringId.h>

namespace atta::io
{
    class Bluetooth
    {
    public:
        struct CreateInfo {
            StringId debugName = StringId("Unnamed io::Bluetooth");
        };

        struct Device {
            std::array<uint8_t, 6> mac;
            std::string name;
            bool connected;
            int16_t rssi;
        };

        Bluetooth(CreateInfo info);
        virtual ~Bluetooth() = default;

        virtual bool start() = 0;
        virtual bool stop() = 0;
        virtual bool update() = 0;
        virtual bool startScan() = 0;
        virtual bool stopScan() = 0;
        virtual bool connect(std::array<uint8_t, 6> mac) = 0;

        std::vector<Device> getDevices() { return _devices; }

    protected:
        StringId _debugName;
        std::vector<Device> _devices;
    };
}

#endif// ATTA_IO_SYSTEM_BLUETOOTH_BLUETOOTH_H
