//--------------------------------------------------
// Atta IO System
// bluetooth.cpp
// Date: 2022-03-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ioSystem/bluetooth/bluetooth.h>

namespace atta::io
{
// MAC parameter, reverse (little-endian)
#define MAC_PARR(x) x[5], x[4], x[3], x[2], x[1], x[0]
#define MAC_PARR_rev(x) &x[5], &x[4], &x[3], &x[2], &x[1], &x[0]

    Bluetooth::Bluetooth(CreateInfo info):
        _debugName(info.debugName)
    {

    }

    std::string Bluetooth::MACToString(std::array<uint8_t, 6> mac)
    {
        std::string out = "--:--:--:--:--:--";
        snprintf(out.data(), out.size()+1, "%02X:%02X:%02X:%02X:%02X:%02X", MAC_PARR(mac));
        return out;
    }

    std::array<uint8_t, 6> Bluetooth::stringToMAC(std::string mac)
    {
        std::array<uint8_t, 6> out;
        const char* in = mac.data();
         
        sscanf(in, "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", MAC_PARR_rev(out));
        return out;
    }
}
