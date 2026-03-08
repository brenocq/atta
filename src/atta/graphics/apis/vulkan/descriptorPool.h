// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/descriptorSetLayout.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class DescriptorPool {
  public:
    DescriptorPool(std::vector<VkDescriptorPoolSize> poolSizes, size_t maxSets);
    ~DescriptorPool();

    VkDescriptorPool getHandle() const;
    std::shared_ptr<Device> getDevice() const;

  private:
    VkDescriptorPool _descriptorPool;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk
