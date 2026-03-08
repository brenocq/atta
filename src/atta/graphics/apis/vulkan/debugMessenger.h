// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/instance.h>

namespace atta::graphics::vk {

class DebugMessenger {
  public:
    DebugMessenger(std::shared_ptr<Instance> instance);
    ~DebugMessenger();

    static VKAPI_ATTR VkBool32 VKAPI_CALL callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                   VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

  private:
    VkDebugUtilsMessengerEXT _debugMessenger;
    std::shared_ptr<Instance> _instance;
};

} // namespace atta::graphics::vk
