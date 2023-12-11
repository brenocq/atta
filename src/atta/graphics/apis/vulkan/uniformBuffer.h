//--------------------------------------------------
// Atta Graphics Module
// uniformBuffer.h
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_UNIFORM_BUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_UNIFORM_BUFFER_H

#include <atta/graphics/apis/vulkan/buffer.h>

namespace atta::graphics::vk {

class UniformBuffer final : public Buffer {
  public:
    UniformBuffer(size_t uniformBufferSize, size_t numInstances);
    ~UniformBuffer();

    void writeInstance(size_t instanceIdx, const std::vector<uint8_t>& data);
    uint32_t getInstanceOffset(size_t instanceIdx);

    size_t getNumInstances() const;
    size_t getSize() const;

  private:
    ///< Align uniform buffer to be used as dynamic
    size_t align(size_t uniformBufferSize);

    void* _mappedData;         ///< Mapped data in the GPU
    size_t _uniformBufferSize; ///< Size of a single uniform buffer in bytes
    size_t _numInstances;      ///< Number of uniform buffer instances
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_UNIFORM_BUFFER_H
