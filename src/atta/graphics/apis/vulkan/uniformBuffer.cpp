//--------------------------------------------------
// Atta Graphics Module
// uniformBuffer.cpp
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/uniformBuffer.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

UniformBuffer::UniformBuffer(size_t size)
    : Buffer({
          size,
          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      }), _size(size) {
    vkMapMemory(_device->getHandle(), _memory, 0, size, 0, &_mappedData);
}

UniformBuffer::~UniformBuffer() {}

void* UniformBuffer::getMappedData() const { return _mappedData; }

size_t UniformBuffer::getSize() const { return _size; }

} // namespace atta::graphics::vk
