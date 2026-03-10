// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/commandPool.h>
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class CommandBuffers {
  public:
    CommandBuffers(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, uint32_t size);
    ~CommandBuffers();

    const std::vector<VkCommandBuffer>& getHandles() const;
    void setHandles(std::vector<VkCommandBuffer> newCommandBuffers);
    std::shared_ptr<Device> getDevice() const;

    /**
     * @brief Begin command buffer recording
     *
     * @param i Command buffer index
     *
     * @return Command buffer vulkan handle
     */
    VkCommandBuffer begin(size_t i);

    /**
     * @brief Get current command buffer being recorded
     *
     * @return Current command buffer being recorded
     */
    VkCommandBuffer getCurrent();

    /**
     * @brief End command buffer recording
     *
     * @param i Command buffer index
     */
    void end(size_t i);

  private:
    std::vector<VkCommandBuffer> _commandBuffers;
    std::shared_ptr<Device> _device;
    std::shared_ptr<CommandPool> _commandPool;
    int _current;
};

} // namespace atta::graphics::vk
