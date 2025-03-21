//--------------------------------------------------
// Atta Graphics Module
// pushConstant.cpp
// Date: 2023-12-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/pushConstant.h>

namespace atta::graphics::vk {

PushConstant::PushConstant(size_t size) : _size(size) {
    _range = {};
    _range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    _range.offset = 0;
    _range.size = _size;
}

void PushConstant::write(VkCommandBuffer commandBuffer, VkPipelineLayout pipelineLayout, const std::vector<uint8_t>& data) {
    if (data.size() != _size) {
        LOG_WARN("gfx::vk::PushConstant", "Could not push constants, data size does not match push constant size");
        return;
    }
    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, _size, data.data());
}

const VkPushConstantRange& PushConstant::getRange() const { return _range; }

size_t PushConstant::getSize() const { return _size; }

} // namespace atta::graphics::vk
