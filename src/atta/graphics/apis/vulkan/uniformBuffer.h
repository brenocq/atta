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
    /**
     * Uniform buffer constructor
     *
     * This class can handle uniform buffer and dynamic uniform buffer. When dynamic uniform buffers are being used, the number of instances need to
     * be specified.
     *
     * @param uniformBufferSize Size of one instance of the uniform buffer in bytes (struct size)
     * @param numInstances Number of uniform buffer instances, maximum number of dynamic uniform buffers
     */
    UniformBuffer(size_t uniformBufferSize, size_t numInstances = 1);
    ~UniformBuffer();

    void writeInstance(const std::vector<uint8_t>& data, size_t instanceIdx = 0);
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
