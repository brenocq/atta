//--------------------------------------------------
// Atta Graphics Module
// stagingBuffer.cpp
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/stagingBuffer.h>

namespace atta::graphics::vk {

StagingBuffer::StagingBuffer(const uint8_t* data, size_t size)
    : Buffer({
          size,
          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      }) {

    // Copy data to staging buffer
    if (data) {
        void* gpuData;
        vkMapMemory(_device->getHandle(), _memory, 0, size, 0, &gpuData);
        memcpy(gpuData, data, size);
        vkUnmapMemory(_device->getHandle(), _memory);
    } else
        LOG_WARN("gfx::vk::StagingBuffer", "Created staging buffer without data");
}

StagingBuffer::~StagingBuffer() {}

} // namespace atta::graphics::vk
