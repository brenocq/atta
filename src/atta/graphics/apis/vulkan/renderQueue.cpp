//--------------------------------------------------
// Atta Graphics Module
// renderQueue.h
// Date: 2023-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/renderQueue.h>

#include <atta/graphics/apis/vulkan/common.h>

namespace atta::graphics::vk {

RenderQueue::RenderQueue() : gfx::RenderQueue() {
    _device = common::getDevice();
    _commandPool = std::make_shared<vk::CommandPool>(_device);
    _commandBuffers = std::make_shared<vk::CommandBuffers>(_device, _commandPool, 1);
    _fence = std::make_shared<vk::Fence>(_device);
}

RenderQueue::~RenderQueue() {
    _fence.reset();
    _commandBuffers.reset();
    _commandPool.reset();
}

void RenderQueue::begin() {
    _fence->wait();
    _fence->reset();
    _commandBuffers->begin(0);
}

void RenderQueue::end() {
    _commandBuffers->end(0);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    // submitInfo.pCommandBuffers = &_commandBuffers->getHandles()[0];
    VkCommandBuffer cmdBuf = _commandBuffers->getHandles()[0];
    submitInfo.pCommandBuffers = &cmdBuf;
    if (vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, _fence->getHandle()) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::RenderQueue", "Failed to submit render queue!");
}

VkCommandBuffer RenderQueue::getCommandBuffer() { return _commandBuffers->getHandles()[0]; }

} // namespace atta::graphics::vk
