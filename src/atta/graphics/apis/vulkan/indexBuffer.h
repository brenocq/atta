//--------------------------------------------------
// Atta Graphics Module
// indexBuffer.h
// Date: 2023-04-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_INDEX_BUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_INDEX_BUFFER_H

#include <atta/graphics/apis/vulkan/buffer.h>
#include <atta/graphics/indexBuffer.h>

namespace atta::graphics::vk {

class IndexBuffer final : public Buffer, public gfx::IndexBuffer {
  public:
    IndexBuffer(const gfx::IndexBuffer::CreateInfo& info);
    ~IndexBuffer();

    void bind() const override;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_INDEX_BUFFER_H
