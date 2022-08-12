//--------------------------------------------------
// Atta IO Module
// interface.h
// Date: 2022-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_INTERFACE_H
#define ATTA_IO_INTERFACE_H

#include <atta/io/bluetooth/bluetooth.h>
#include <atta/io/camera/camera.h>
#include <atta/io/serial/serial.h>

namespace atta::io {

void startUp();
void shutDown();
void update();

// Used to create objects (camera/bluetooth/uart/...) based on the current operating system
// e.g.: io::create<io::Camera>(cameraInfo) will create io::LinuxCameraUSB or
// io::WindowsCameraUSB or ... depending on the current operating system
template <typename T, typename... Args>
std::shared_ptr<T> create(Args... args);

} // namespace atta::io

#include <atta/io/manager.h>

namespace atta::io {

template <typename T, typename... Args>
std::shared_ptr<T> create(Args... args) {
    return Manager::getInstance().createImpl<T>(args...);
}

} // namespace atta::io

#endif // ATTA_IO_INTERFACE_H
