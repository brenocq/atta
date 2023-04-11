//--------------------------------------------------
// Atta Graphics Module
// buffer.h
// Date: 2023-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_VULKAN_BUFFER_H
#define ATTA_GRAPHICS_APIS_VULKAN_BUFFER_H

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/device.h>

namespace atta::graphics::vk {

class Buffer {
  public:
    struct CreateInfo {
        size_t size;
        VkBufferUsageFlags usage;
        VkMemoryPropertyFlags properties;
    };
    Buffer(const CreateInfo& info);
    ~Buffer();

    VkBuffer getHandle() const;
    VkDeviceMemory getMemoryHandle() const;

    static void copy(std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst);

  protected:
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void setData(uint8_t* data);

    VkBuffer _buffer;
    VkDeviceMemory _memory;
    size_t _bufferSize;
    std::shared_ptr<Device> _device;
};

} // namespace atta::graphics::vk

#endif // ATTA_GRAPHICS_APIS_VULKAN_BUFFER_H
