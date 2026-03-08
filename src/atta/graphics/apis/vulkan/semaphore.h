// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class Semaphore {
  public:
    Semaphore(std::shared_ptr<Device> device);
    ~Semaphore();

    VkSemaphore getHandle() const;
    std::shared_ptr<Device> getDevice() const;

  private:
    VkSemaphore _semaphore;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk
