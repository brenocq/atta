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
    // TODO do not create framebuffer if already created for this render pass
    _framebuffer = std::dynamic_pointer_cast<vk::Framebuffer>(_renderPass->getFramebuffer());
    _framebuffer->create(std::dynamic_pointer_cast<vk::RenderPass>(_renderPass));

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
    inputAssembly.topology = convert(info.primitive);
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
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_FALSE;
    depthStencil.depthWriteEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;
    if (_framebuffer->hasDepthAttachment() && _enableDepthTest) {
        depthStencil.depthTestEnable = VK_TRUE;
        depthStencil.depthWriteEnable = VK_TRUE;
        depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.minDepthBounds = 0.0f;
        depthStencil.maxDepthBounds = 1.0f;
    }
    if (_framebuffer->hasStencilAttachment()) {
        depthStencil.stencilTestEnable = VK_FALSE;
        depthStencil.front = {};
        depthStencil.back = {};
    }

    // Color blend
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    if (gfx::Image::getNumChannels(_framebuffer->getImage(0)->getFormat()) == 4) {
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    // Dynamic state
    std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // Descriptor set layouts
    std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSetLayouts;
    bool createDefaultImageGroupPerFrame = false;
    bool createDefaultImageGroupPerDraw = false;
    {
        // Uniform
        std::vector<DescriptorSetLayout::Binding> bindings;
        bindings.push_back({0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT});
        _uniformDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(bindings);
        descriptorSetLayouts.push_back(_uniformDescriptorSetLayout);
    }
    {
        // Per frame images
        std::vector<DescriptorSetLayout::Binding> bindings;
        for (const auto& element : _shader->getPerFrameImageLayout().getElements()) {
            uint32_t bindingIdx = bindings.size();
            if (element.type == BufferLayout::Element::Type::SAMPLER_2D || element.type == BufferLayout::Element::Type::SAMPLER_CUBE)
                bindings.push_back(
                    {bindingIdx, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT});
        }
        if (bindings.size()) {
            _perFrameImageDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(bindings);
            descriptorSetLayouts.push_back(_perFrameImageDescriptorSetLayout);
            createDefaultImageGroupPerFrame = true;
        }
    }
    {
        // Per draw images
        std::vector<DescriptorSetLayout::Binding> bindings;
        for (const auto& element : _shader->getPerDrawImageLayout().getElements()) {
            uint32_t bindingIdx = bindings.size();
            if (element.type == BufferLayout::Element::Type::SAMPLER_2D || element.type == BufferLayout::Element::Type::SAMPLER_CUBE)
                bindings.push_back(
                    {bindingIdx, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT});
        }
        if (bindings.size()) {
            _perDrawImageDescriptorSetLayout = std::make_shared<DescriptorSetLayout>(bindings);
            descriptorSetLayouts.push_back(_perDrawImageDescriptorSetLayout);
            createDefaultImageGroupPerDraw = true;
        }
    }

    // Push constant
    std::shared_ptr<PushConstant> pushConstant = std::dynamic_pointer_cast<vk::Shader>(_shader)->getPushConstant();

    // Pipeline layout
    _pipelineLayout = std::make_shared<PipelineLayout>(descriptorSetLayouts, pushConstant);

    // Descriptor set
    const uint32_t maxSets = 254; // Maximum number of image groups
    std::vector<VkDescriptorPoolSize> poolSizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1},                   // Up to 1 uniform buffer
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 8 * maxSets}, // Up to 8 sampler2D/samplerCube perDraw/perFrame
    };
    _descriptorPool = std::make_shared<DescriptorPool>(poolSizes, maxSets);
    _descriptorSets = std::make_shared<DescriptorSets>(_descriptorPool, _uniformDescriptorSetLayout, _pipelineLayout, 1);
    _descriptorSets->update(0, std::dynamic_pointer_cast<vk::Shader>(_shader)->getUniformBuffer());

    // Create default image group descriptor sets (pink images)
    if (createDefaultImageGroupPerFrame)
        createImageGroup(ImageGroupType::PER_FRAME, "atta::DefaultPerFramePink");
    if (createDefaultImageGroupPerDraw)
        createImageGroup(ImageGroupType::PER_DRAW, "atta::DefaultPerDrawPink");

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
    _framebuffer.reset();
}

void Pipeline::begin() {
    // Bind shader
    _shader->bind();

    // Bind framebuffer
    _framebuffer->bind();

    // Bind pipeline
    VkCommandBuffer commandBuffer = std::dynamic_pointer_cast<vk::RenderQueue>(_renderPass->getRenderQueue())->getCommandBuffer();
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);

    // Configure viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = _framebuffer->getWidth();
    viewport.height = _framebuffer->getHeight();
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    // Configure scissor
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {(uint32_t)viewport.width, (uint32_t)viewport.height};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Bind uniform buffer
    _descriptorSets->bind(commandBuffer, 0);

    // Bind pink image groups
    if (_imageGroups.find("atta::DefaultPerFramePink") != _imageGroups.end())
        setImageGroup("atta::DefaultPerFramePink");
    if (_imageGroups.find("atta::DefaultPerDrawPink") != _imageGroups.end())
        setImageGroup("atta::DefaultPerDrawPink");
}

void Pipeline::end() {
    // Write data to uniform buffer
    std::dynamic_pointer_cast<vk::Shader>(_shader)->pushUniformBuffer();

    // Unbind framebuffer
    _framebuffer->unbind();

    // Unbind shader
    _shader->unbind();
}

void Pipeline::resize(uint32_t width, uint32_t height) {
    _framebuffer->resize(width, height);
    _framebuffer->create(std::dynamic_pointer_cast<vk::RenderPass>(_renderPass));
}

void Pipeline::renderMesh(StringId meshSid, size_t numVertices) {
    std::shared_ptr<vk::Mesh> mesh = std::dynamic_pointer_cast<vk::Mesh>(Manager::getInstance().getMeshes().at(meshSid));
    if (mesh) {
        // Get command buffer
        VkCommandBuffer commandBuffer = std::dynamic_pointer_cast<vk::RenderQueue>(_renderPass->getRenderQueue())->getCommandBuffer();

        // Write push constants
        std::dynamic_pointer_cast<vk::Shader>(_shader)->pushConstants(commandBuffer, _pipelineLayout);

        // Draw mesh
        mesh->draw(commandBuffer, numVertices);
    } else
        LOG_WARN("gfx::vk::Pipeline", "Could not render mesh [w]$0[], mesh not found", meshSid);
}

void* Pipeline::getImGuiTexture() const {
    return reinterpret_cast<void*>(std::static_pointer_cast<Image>(_framebuffer->getImage(0))->getImGuiImage());
}

void Pipeline::createImageGroup(ImageGroupType type, std::string name) {
    if (_imageGroups.find(name) != _imageGroups.end()) {
        LOG_WARN("gfx::vk::Pipeline", "Could not create image group [w]$0[], this group already exists", name);
        return;
    }
    if (type == ImageGroupType::PER_FRAME && _shader->getPerFrameImageLayout().empty()) {
        LOG_WARN("gfx::vk::Pipeline", "Could not create image group [w]$0[], shader does not use [w]perFrame[] images", name);
        return;
    }
    if (type == ImageGroupType::PER_DRAW && _shader->getPerDrawImageLayout().empty()) {
        LOG_WARN("gfx::vk::Pipeline", "Could not create image group [w]$0[], shader does not use [w]perDraw[] images", name);
        return;
    }

    std::shared_ptr<DescriptorSetLayout> descriptorSetLayout =
        type == ImageGroupType::PER_FRAME ? _perFrameImageDescriptorSetLayout : _perDrawImageDescriptorSetLayout;

    // Allocate descriptor set
    std::shared_ptr<DescriptorSets> descriptorSet = std::make_shared<DescriptorSets>(_descriptorPool, descriptorSetLayout, _pipelineLayout, 1);

    // Save descriptor set
    ImageGroupInfo info;
    info.type = type;
    info.data = {};
    info.descriptorSet = descriptorSet;
    _imageGroups[name] = info;

    // Update images to pink
    updateImageGroup(name, {});
}

void Pipeline::updateImageGroup(std::string name, ImageGroup imageGroup) {
    if (_imageGroups.find(name) == _imageGroups.end()) {
        LOG_WARN("gfx::vk::Pipeline", "Could not update image group [w]$0[], this group was not created", name);
        return;
    }

    // Get image group descriptor set
    ImageGroupInfo& imageGroupInfo = _imageGroups[name];
    const BufferLayout& imageLayout =
        imageGroupInfo.type == ImageGroupType::PER_FRAME ? _shader->getPerFrameImageLayout() : _shader->getPerDrawImageLayout();

    // Update descriptor set
    size_t binding = 0;
    for (const auto& element : imageLayout.getElements()) {
        // Find image in image group
        std::shared_ptr<Image> image;
        for (const auto& imageItem : imageGroup)
            if (imageItem.name == element.name && imageItem.image)
                image = std::dynamic_pointer_cast<vk::Image>(imageItem.image);

        // Update descriptor: if no image, use a default pink image
        if (image != nullptr)
            imageGroupInfo.descriptorSet->update(binding++, image);
        else {
            // Set default pink image to this binding
            std::shared_ptr<vk::Image> defaultImage;
            StringId defaultImageSid{};
            if (element.type == BufferLayout::Element::Type::SAMPLER_2D)
                defaultImageSid = "atta::gfx::pink";
            else if (element.type == BufferLayout::Element::Type::SAMPLER_CUBE)
                defaultImageSid = "atta::gfx::pinkCubemap";
            else {
                LOG_ERROR("gfx::vk::Pipeline", "Could not set default image for [w]$0[], unknown image type", element.name);
                continue;
            }
            defaultImage = std::dynamic_pointer_cast<vk::Image>(gfx::Manager::getInstance().getImage(defaultImageSid));
            imageGroupInfo.descriptorSet->update(binding++, defaultImage);
        }
    }

    // Update image groups
    imageGroupInfo.data = imageGroup;
}

void Pipeline::destroyImageGroup(std::string name) {
    // Remove image group
    _imageGroups.erase(name);
}

void Pipeline::setImageGroup(const char* name) {
    if (_renderPass->getRenderQueue() == nullptr) {
        LOG_WARN("gfx::vk::Pipeline", "Can not set image group outside render pass");
        return;
    }
    if (_imageGroups.find(name) == _imageGroups.end()) {
        LOG_WARN("gfx::vk::Pipeline", "Can not set image group [w]$0[], image group not found", name);
        return;
    }

    // Get command buffer
    VkCommandBuffer commandBuffer = std::dynamic_pointer_cast<vk::RenderQueue>(_renderPass->getRenderQueue())->getCommandBuffer();

    // Bind descriptor set
    uint32_t setIdx = 1;
    if (!_shader->getPerFrameImageLayout().empty() && _imageGroups[name].type == ImageGroupType::PER_DRAW)
        setIdx = 2;
    _imageGroups[name].descriptorSet->bind(commandBuffer, setIdx);
}

VkPipeline Pipeline::getHandle() const { return _pipeline; }
std::shared_ptr<PipelineLayout> Pipeline::getPipelineLayout() const { return _pipelineLayout; }

VkPrimitiveTopology Pipeline::convert(Primitive topology) {
    switch (topology) {
        case Primitive::POINT:
            return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
        case Primitive::LINE:
            return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
        case Primitive::TRIANGLE:
            return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
    return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}

} // namespace atta::graphics::vk
