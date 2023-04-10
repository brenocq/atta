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
    UniformBuffer(size_t size);
    ~UniformBuffer();

    void* getMappedData() const;
    size_t getSize() const;

  private:
    void* _mappedData; ///< Mapped data in the GPU
    size_t _size;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_UNIFORM_BUFFER_H
