// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/apis/vulkan/common.h>

namespace atta::graphics::vk {

class Instance {
  public:
    Instance();
    ~Instance();

    bool wasCreated() const;

    VkInstance getHandle() const;
    uint32_t getApiVersion() const;

    /// True if there is a GPU with vulkan support
    bool hasCompatibleGPU() const;

  private:
    void printAvailableExtensions();
    void printAvailableLayers();

    std::vector<VkExtensionProperties> getAvailableExtensions() const;
    std::vector<VkLayerProperties> getAvailableLayers() const;

    std::vector<const char*> getEnabledExtensions();
    std::vector<const char*> getEnabledLayers();

    VkInstance _instance;
    bool _wasCreated;
    VkDebugUtilsMessengerEXT debugMessenger;
    uint32_t _apiVersion;
};

} // namespace atta::graphics::vk
