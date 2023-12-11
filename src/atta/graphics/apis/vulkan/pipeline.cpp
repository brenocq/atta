//--------------------------------------------------
// Atta Graphics Module
// pipeline.cpp
// Date: 2023-04-04
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/mesh.h>
#include <atta/graphics/apis/vulkan/pipeline.h>
#include <atta/graphics/apis/vulkan/renderQueue.h>
#include <atta/graphics/apis/vulkan/shader.h>
#include <atta/graphics/apis/vulkan/vulkanAPI.h>
#include <atta/graphics/interface.h>

namespace atta::graphics::vk {

Pipeline::Pipeline(const gfx::Pipeline::CreateInfo& info) : gfx::Pipeline(info), _device(common::getDevice()) {
    // Create framebuffer
    _framebuffers.push_back(std::dynamic_pointer_cast<vk::Framebuffer>(_renderPass->getFramebuffer()));
    _framebuffers[0]->create(std::dynamic_pointer_cast<vk::RenderPass>(_renderPass));

    // Vertex input
    BufferLayout layout = _shader->getVertexBufferLayout();
    auto bindingDescription = VertexBuffer::getBindingDescription(layout);
    auto attributeDescriptions = VertexBuffer::getAttributeDescriptions(layout);
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport state
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    // Rasterizer
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;

    // Depth stencil
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.depthTestEnable = VK_FALSE;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;
    if (_framebuffers[0]->hasDepthAttachment()) {
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
    }
    if (_framebuffers[0]->hasStencilAttachment()) {
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};
    }

    // Color blend
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE; // XXX blend is disabled for now
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    // Dynamic state
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // Descriptor set layout
    std::vector<DescriptorSetLayout::Binding> bindings;
    bindings.push_back({0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT});
    _descriptorSetLayout = std::make_shared<DescriptorSetLayout>(bindings);

    // Pipeline layout
    _pipelineLayout = std::make_shared<PipelineLayout>(_descriptorSetLayout);

    // Descriptor set
    _descriptorPool = std::make_shared<DescriptorPool>(bindings, 1);
    _descriptorSets = std::make_shared<DescriptorSets>(_descriptorPool, _descriptorSetLayout, _pipelineLayout, 1);
    _descriptorSets->update(0, std::dynamic_pointer_cast<vk::Shader>(_shader)->getUniformBuffer());

    // Create Pipeline
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages = std::dynamic_pointer_cast<vk::Shader>(_shader)->getShaderStages();
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = shaderStages.size();
    pipelineInfo.pStages = shaderStages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = _pipelineLayout->getHandle();
    pipelineInfo.renderPass = std::dynamic_pointer_cast<vk::RenderPass>(_renderPass)->getHandle();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(_device->getHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
        LOG_ERROR("gfx::vk::Pipeline", "Failed to create pipeline!");
}

Pipeline::~Pipeline() {
    if (_pipeline != VK_NULL_HANDLE)
        vkDestroyPipeline(_device->getHandle(), _pipeline, nullptr);
    _framebuffers.clear();
}

void Pipeline::begin(std::shared_ptr<gfx::RenderQueue> renderQueue) {
    _renderQueue = renderQueue;

    // Bind shader
    _shader->bind();

    // Bind framebuffer
    _framebuffers[0]->bind();

    // Bind pipeline
    VkCommandBuffer commandBuffer = std::dynamic_pointer_cast<vk::RenderQueue>(_renderQueue)->getCommandBuffer();
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);

    // Configure viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = _renderPass->getFramebuffer()->getWidth();
    viewport.height = _renderPass->getFramebuffer()->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    // Configure scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {(uint32_t)viewport.width, (uint32_t)viewport.height};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Bind descriptor sets
    _descriptorSets->bind(commandBuffer, 0);
}

void Pipeline::end() {
    _renderQueue.reset();
    _shader->unbind();
}

void Pipeline::renderMesh(StringId meshSid) {
    std::shared_ptr<vk::Mesh> mesh = std::dynamic_pointer_cast<vk::Mesh>(Manager::getInstance().getMeshes().at(meshSid));
    if (mesh) {
        VkCommandBuffer commandBuffer = std::dynamic_pointer_cast<vk::RenderQueue>(_renderQueue)->getCommandBuffer();
        mesh->draw(commandBuffer);
    } else
        LOG_WARN("gfx::vk::Pipeline", "Could not render mesh [w]$0[], mesh not found", meshSid);
}

void* Pipeline::getImGuiTexture() const {
    return reinterpret_cast<void*>(std::static_pointer_cast<Image>(_renderPass->getFramebuffer()->getImage(0))->getImGuiImage());
}

VkPipeline Pipeline::getHandle() const { return _pipeline; }
std::shared_ptr<PipelineLayout> Pipeline::getPipelineLayout() const { return _pipelineLayout; }

} // namespace atta::graphics::vk
