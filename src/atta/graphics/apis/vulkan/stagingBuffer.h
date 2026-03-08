// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_GRAPHICS_APIS_VULKAN_STAGING_BUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_STAGING_BUFFER_H

#include <atta/graphics/apis/vulkan/buffer.h>

namespace atta::graphics::vk {

class StagingBuffer final : public Buffer {
  public:
    StagingBuffer(const uint8_t* data, size_t size);
    ~StagingBuffer();
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_STAGING_BUFFER_H
