// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_GRAPHICS_APIS_VULKAN_COMMON_H
#define ATTA_GRAPHICS_APIS_VULKAN_COMMON_H

#include <volk.h>

namespace atta::graphics::vk {
class Device;
class CommandBuffers;
class CommandPool;
} // namespace atta::graphics::vk

namespace atta::graphics::vk::common {

std::string toString(VkResult result);
std::string toString(VkFormat format);
std::shared_ptr<vk::Device> getDevice();
std::shared_ptr<vk::CommandBuffers> getCommandBuffers();
std::shared_ptr<vk::CommandPool> getCommandPool();

} // namespace atta::graphics::vk::common

#endif // ATTA_GRAPHICS_APIS_VULKAN_COMMON_H
