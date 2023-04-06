//--------------------------------------------------
// Atta Graphics Module
// commandBuffers.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/commandBuffers.h>

namespace atta::graphics::vk {

CommandBuffers::CommandBuffers(std::shared_ptr<Device> device, std::shared_ptr<CommandPool> commandPool, uint32_t size)
    : _device(device), _commandPool(commandPool), _current(-1) {
    _commandBuffers.resize(size);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool->getHandle();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

    if (vkAllocateCommandBuffers(_device->getHandle(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::CommandBuffers", "Failed to create command buffers!");
}

CommandBuffers::~CommandBuffers() {
    vkFreeCommandBuffers(_device->getHandle(), _commandPool->getHandle(), static_cast<uint32_t>(_commandBuffers.size()), _commandBuffers.data());
}

std::vector<VkCommandBuffer> CommandBuffers::getHandles() const { return _commandBuffers; }
void CommandBuffers::setHandles(std::vector<VkCommandBuffer> newCommandBuffers) { _commandBuffers = newCommandBuffers; }
std::shared_ptr<Device> CommandBuffers::getDevice() const { return _device; }

VkCommandBuffer CommandBuffers::begin(size_t i) {
    // Make sure it is able to record
    vkResetCommandBuffer(_commandBuffers[i], 0);

    // Start recording
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::CommandBuffers", "Failed to begin recording command buffer!");

    _current = i;
    return _commandBuffers[i];
}

VkCommandBuffer CommandBuffers::get() {
    if (_current >= 0)
        return _commandBuffers[_current];
    else
        return VK_NULL_HANDLE;
}

void CommandBuffers::end(size_t i) {
    if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::CommandBuffers", "Failed to record command buffer!");
    _current = -1;
}

} // namespace atta::graphics::vk
