//--------------------------------------------------
// Atta Graphics Module
// uniformBuffer.cpp
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/uniformBuffer.h>

#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

UniformBuffer::UniformBuffer(size_t uniformBufferSize, size_t numInstances)
    : Buffer({
          align(uniformBufferSize) * numInstances,
          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      }),
      _uniformBufferSize(align(uniformBufferSize)), _numInstances(numInstances) {
    vkMapMemory(_device->getHandle(), _memory, 0, _uniformBufferSize * _numInstances, 0, &_mappedData);
}

UniformBuffer::~UniformBuffer() { vkUnmapMemory(_device->getHandle(), _memory); }

void UniformBuffer::writeInstance(const std::vector<uint8_t>& data, size_t instanceIdx) {
    memcpy((uint8_t*)_mappedData + getInstanceOffset(instanceIdx), data.data(), data.size());
}

uint32_t UniformBuffer::getInstanceOffset(size_t instanceIdx) { return _uniformBufferSize * instanceIdx; }

size_t UniformBuffer::getNumInstances() const { return _numInstances; }

size_t UniformBuffer::getSize() const { return _uniformBufferSize; }

size_t UniformBuffer::align(size_t uniformBufferSize) { return (uniformBufferSize + 64 - 1) & ~(64 - 1); }

} // namespace atta::graphics::vk
