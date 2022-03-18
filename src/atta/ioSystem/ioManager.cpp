//--------------------------------------------------
// Atta IO System
// ioManager.cpp
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ioSystem/ioManager.h>
#include <atta/ioSystem/camera/linuxCamera.h>
#include <atta/ioSystem/serial/linuxSerial.h>
#include <atta/ioSystem/bluetooth/linuxBluetooth.h>

namespace atta::io
{
    IOManager& IOManager::getInstance()
    {
        static IOManager instance;
        return instance;
    }

    void IOManager::startUp() { getInstance().startUpImpl(); }
    void IOManager::startUpImpl()
    {
    }

    void IOManager::shutDown() { getInstance().shutDownImpl(); }
    void IOManager::shutDownImpl()
    {
    }

    void IOManager::update() { getInstance().updateImpl(); }
    void IOManager::updateImpl()
    {
    }

    //---------- Register OS specific implementations ----------//
    template <>
    std::shared_ptr<io::Camera> IOManager::createImpl<io::Camera>(io::Camera::CreateInfo info)
    {
#ifdef ATTA_OS_LINUX
        return std::static_pointer_cast<io::Camera>(std::make_shared<io::LinuxCamera>(info));
#else
        LOG_WARN("IOManager", "io::Camera not implemented for this operating system");
        return nullptr;
#endif
    }

    template <>
    std::shared_ptr<io::Serial> IOManager::createImpl<io::Serial>(io::Serial::CreateInfo info)
    {
#ifdef ATTA_OS_LINUX
        return std::static_pointer_cast<io::Serial>(std::make_shared<io::LinuxSerial>(info));
#else
        LOG_WARN("IOManager", "io::Serial not implemented for this operating system");
        return nullptr;
#endif
    }

    template <>
    std::shared_ptr<io::Bluetooth> IOManager::createImpl<io::Bluetooth>(io::Bluetooth::CreateInfo info)
    {
#if defined(ATTA_OS_LINUX)
    #if defined(ATTA_SYSTEMD_SUPPORT)
        return std::static_pointer_cast<io::Bluetooth>(std::make_shared<io::LinuxBluetooth>(info));
    #else
        LOG_WARN("IOManager", "io::Bluetooth not supported when [w]systemd[] is not installed");
        return nullptr;
    #endif
#else
        LOG_WARN("IOManager", "io::Bluetooth not implemented for this operating system");
        return nullptr;
#endif
    }
}
