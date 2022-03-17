//--------------------------------------------------
// Atta IO System
// linuxBluetooth.h
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_BLUETOOTH_LINUX_BLUETOOTH_H
#define ATTA_IO_SYSTEM_BLUETOOTH_LINUX_BLUETOOTH_H
#ifdef ATTA_OS_LINUX
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

        static int bluezScanCallback(sd_bus_message* m, void* user, sd_bus_error* err);

    private:
        std::string _adapter;
        sd_bus* _bus;
        sd_bus_slot* _scanSlot;
    };
}

#endif// ATTA_OS_LINUX
#endif// ATTA_IO_SYSTEM_BLUETOOTH_LINUX_BLUETOOTH_H
