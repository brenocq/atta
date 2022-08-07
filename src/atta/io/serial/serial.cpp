//--------------------------------------------------
// Atta IO Module
// serial.cpp
// Date: 2021-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/io/serial/serial.h>

namespace atta::io {
Serial::Serial(CreateInfo info)
    : _deviceName(info.deviceName), _baudRate(info.baudRate), _parityBit(info.parityBit), _twoStopBits(info.twoStopBits), _rtsCts(info.rtsCts),
      _timeout(info.timeout), _debugName(info.debugName) {
    if (_timeout < 0.0f) {
        LOG_WARN("io::Serial", "Timeout must be non-negative, using 0.0 seconds as timeout");
        _timeout = 0.0f;
    }
}

std::vector<std::string> Serial::getAvailableDeviceNames() {
    std::vector<std::string> deviceNames;

#ifdef ATTA_OS_LINUX
    if (fs::exists("/dev/serial/by-id"))
        for (auto& entry : fs::directory_iterator("/dev/serial/by-id"))
            deviceNames.push_back(entry.path().string());
#endif
    return deviceNames;
}
} // namespace atta::io
