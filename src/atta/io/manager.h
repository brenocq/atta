//--------------------------------------------------
// Atta IO Module
// manager.h
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_MANAGER_H
#define ATTA_IO_MANAGER_H

#include <atta/io/bluetooth/bluetooth.h>
#include <atta/io/camera/camera.h>
#include <atta/io/serial/serial.h>

namespace atta::io {

class Manager final {
  public:
    static Manager& getInstance();

    friend void startUp();
    friend void shutDown();
    friend void update();
    template <typename T, typename... Args>
    friend std::shared_ptr<T> create(Args... args);

  private:
    void startUpImpl();
    void shutDownImpl();
    void updateImpl();
    template <typename T, typename... Args>
    std::shared_ptr<T> createImpl(Args... args);
};

} // namespace atta::io

#include <atta/io/manager.inl>
#endif // ATTA_IO_MANAGER_H
