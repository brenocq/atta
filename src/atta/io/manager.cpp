//--------------------------------------------------
// Atta IO Module
// manager.cpp
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/io/bluetooth/linuxBluetooth.h>
#include <atta/io/camera/linuxCamera.h>
#include <atta/io/manager.h>
#include <atta/io/serial/linuxSerial.h>

namespace atta::io {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {}

void Manager::shutDownImpl() {}

void Manager::updateImpl() {}

//---------- Register OS specific implementations ----------//
template <>
std::shared_ptr<io::Camera> Manager::createImpl<io::Camera>(io::Camera::CreateInfo info) {
#ifdef ATTA_OS_LINUX
    return std::static_pointer_cast<io::Camera>(std::make_shared<io::LinuxCamera>(info));
#else
    LOG_WARN("io::Manager", "io::Camera not implemented for this operating system");
    return nullptr;
#endif
}

template <>
std::shared_ptr<io::Serial> Manager::createImpl<io::Serial>(io::Serial::CreateInfo info) {
#ifdef ATTA_OS_LINUX
    return std::static_pointer_cast<io::Serial>(std::make_shared<io::LinuxSerial>(info));
#else
    LOG_WARN("io::Manager", "io::Serial not implemented for this operating system");
    return nullptr;
#endif
}

template <>
std::shared_ptr<io::Bluetooth> Manager::createImpl<io::Bluetooth>(io::Bluetooth::CreateInfo info) {
#if defined(ATTA_OS_LINUX)
#if defined(ATTA_SYSTEMD_SUPPORT)
    return std::static_pointer_cast<io::Bluetooth>(std::make_shared<io::LinuxBluetooth>(info));
#else
    LOG_WARN("io::Manager", "io::Bluetooth not supported when [w]systemd[] is not installed");
    return nullptr;
#endif
#else
    LOG_WARN("io::Manager", "io::Bluetooth not implemented for this operating system");
    return nullptr;
#endif
}

} // namespace atta::io
