//--------------------------------------------------
// Atta IO Module
// manager.h
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_IO_MANAGER_H
#define ATTA_IO_IO_MANAGER_H

#include <atta/io/bluetooth/bluetooth.h>
#include <atta/io/camera/camera.h>
#include <atta/io/serial/serial.h>

namespace atta::io {

class Manager final {
  public:
    static Manager& getInstance();
    static void startUp();
    static void shutDown();

    static void update();

    // Used to create objects (camera/bluetooth/uart/...) based on the current operating system
    // e.g.: io::Manager::create<io::Camera>(cameraInfo) will create io::LinuxCameraUSB or
    // io::WindowsCameraUSB or ... depending on the current operating system
    template <typename T, typename... Args>
    static std::shared_ptr<T> create(Args... args) {
        return getInstance().createImpl<T>(args...);
    }

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl();
    template <typename T, typename... Args>
    std::shared_ptr<T> createImpl(Args... args);
};

} // namespace atta::io

#include <atta/io/manager.inl>
#endif // ATTA_IO_IO_MANAGER_H
