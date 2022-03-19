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
        struct CreateInfo
        {
            StringId debugName = StringId("Unnamed io::Bluetooth");
        };

        enum class CharFlags
        {
            BROADCAST         = 1 << 0,
            READ              = 1 << 1,
            WRITE_NO_RESPONSE = 1 << 2,
            WRITE             = 1 << 3,
            NOTIFY            = 1 << 4,
            INFICATE          = 1 << 5,
            SIGNED_WRITE      = 1 << 6,
            EXTENDED          = 1 << 7
        };

        struct Char
        {
            std::string uuid;
            CharFlags flags;
            bool notifying;
        };

        struct Service
        {
            std::string uuid;
            std::vector<Char> _char;
        };

        struct Device
        {
            std::array<uint8_t, 6> mac;
            std::string name;
            bool connected;
            int16_t rssi;
            std::vector<Service> services;
        };

        Bluetooth(CreateInfo info);
        virtual ~Bluetooth() = default;

        virtual bool start() = 0;
        virtual bool stop() = 0;
        virtual bool update() = 0;
        virtual bool startScan() = 0;
        virtual bool stopScan() = 0;
        virtual bool connect(std::array<uint8_t, 6> mac) = 0;
        virtual bool disconnect(std::array<uint8_t, 6> mac) = 0;

        std::vector<Device> getDevices() { return _devices; }
        static std::string MACToString(std::array<uint8_t, 6> mac);
        static std::array<uint8_t, 6> stringToMAC(std::string mac);
    protected:
        StringId _debugName;
        std::vector<Device> _devices;
    };

    inline Bluetooth::CharFlags operator|(Bluetooth::CharFlags a, Bluetooth::CharFlags b)
    {
        return static_cast<Bluetooth::CharFlags>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline Bluetooth::CharFlags operator&(Bluetooth::CharFlags a, Bluetooth::CharFlags b)
    {
        return static_cast<Bluetooth::CharFlags>(static_cast<int>(a) & static_cast<int>(b));
    }
}

#endif// ATTA_IO_SYSTEM_BLUETOOTH_BLUETOOTH_H
