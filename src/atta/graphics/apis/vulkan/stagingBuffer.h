//--------------------------------------------------
// Atta Graphics Module
// stagingBuffer.h
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
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
