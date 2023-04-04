//--------------------------------------------------
// Atta Graphics Module
// common.h
// Date: 2023-04-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_COMMON_H
#define ATTA_GRAPHICS_APIS_VULKAN_COMMON_H

#include "vulkan/vulkan.h"

namespace atta::graphics::vk {

std::string toString(VkResult result);
std::string toString(VkFormat format);

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_COMMON_H
