//--------------------------------------------------
// Atta Graphics Module
// fence.cpp
// Date: 2023-04-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/fence.h>

namespace atta::graphics::vk {

Fence::Fence(std::shared_ptr<Device> device) : _device(device) {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(_device->getHandle(), &fenceInfo, nullptr, &_fence) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Fence", "Failed to create fence!");
}

Fence::~Fence() {
    if (_fence != VK_NULL_HANDLE)
        vkDestroyFence(_device->getHandle(), _fence, nullptr);
}

VkFence Fence::getHandle() { return _fence; }

std::shared_ptr<Device> Fence::getDevice() const { return _device; }

void Fence::reset() {
    if (vkResetFences(_device->getHandle(), 1, &_fence) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Fence", "Failed to reset fence!");
}

void Fence::wait(uint64_t timeout) const {
    if (vkWaitForFences(_device->getHandle(), 1, &_fence, VK_TRUE, timeout) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Fence", "Failed to wait for fence!");
}

} // namespace atta::graphics::vk
