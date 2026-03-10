// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

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
