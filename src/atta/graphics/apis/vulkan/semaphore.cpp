//--------------------------------------------------
// Atta Graphics Module
// semaphore.cpp
// Date: 2023-04-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/semaphore.h>

namespace atta::graphics::vk {

Semaphore::Semaphore(std::shared_ptr<Device> device) : _device(device) {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    if (vkCreateSemaphore(_device->getHandle(), &semaphoreInfo, nullptr, &_semaphore) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Semaphore", "Failed to create semaphore!");
}

Semaphore::~Semaphore() {
    if (_semaphore != VK_NULL_HANDLE)
        vkDestroySemaphore(_device->getHandle(), _semaphore, nullptr);
}

VkSemaphore Semaphore::getHandle() const { return _semaphore; }

std::shared_ptr<Device> Semaphore::getDevice() const { return _device; }

} // namespace atta::graphics::vk
