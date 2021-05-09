//--------------------------------------------------
// Robot Simulator
// guiPipeline.cpp
// Date: 2020-11-27
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/guiPipeline.h>
#include <atta/graphics/gui/guiVertex.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/helpers/log.h>

namespace atta
{
	GuiPipeline::GuiPipeline(
					std::shared_ptr<vk::Device> device, 
					std::shared_ptr<Window> window,
					std::shared_ptr<vk::SwapChain> swapChain,
					std::shared_ptr<GuiUniformBuffer> uniformBuffer,
					std::shared_ptr<guib::FontLoader> _fontLoader):
		_device(device)
	{
		//---------- Get swapchain images ----------//
		_imageViews = swapChain->getImageViews();
		_imageFormat = swapChain->getImageFormat();
		_imageExtent = swapChain->getImageExtent();

		//---------- Depth buffers ----------//
		_depthBuffers.resize(_imageViews.size());
		for(size_t i=0; i<_depthBuffers.size(); i++)
			_depthBuffers[i] = std::make_shared<vk::DepthBuffer>(_device, _imageExtent, true);// Depth buffer with disabled multisampling

		// Not using colors buffer because no multisampling is being performed
		_colorBuffers.resize(_imageViews.size());
		for(size_t i=0; i<_colorBuffers.size(); i++)
			_colorBuffers[i] = std::make_shared<vk::ColorBuffer>(_device, _imageExtent, _imageFormat);

		//---------- Render pass ----------//
		_renderPass = std::make_shared<GuiRenderPass>(_device, _imageFormat, _depthBuffers[0]->getFormat());

		//---------- FrameBuffers ----------//
		_frameBuffers.resize(_imageViews.size());
		for(int i = 0; i < (int)_frameBuffers.size(); i++)
			_frameBuffers[i] = std::make_shared<GuiFrameBuffer>(
					_device, 
					_imageViews[i], _depthBuffers[i]->getImageView(), _colorBuffers[i]->getImageView(), 
					_renderPass, _imageExtent);

		//---------- Shaders ----------//
		_vertShaderModule = std::make_shared<vk::ShaderModule>(_device, "/usr/include/atta/assets/shaders/gui/guiShader.vert.spv");
		_fragShaderModule = std::make_shared<vk::ShaderModule>(_device, "/usr/include/atta/assets/shaders/gui/guiShader.frag.spv");

		// Vert shader
		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = _vertShaderModule->handle();
		vertShaderStageInfo.pName = "main";

		// Frag shader
		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = _fragShaderModule->handle();
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

		//---------- Fixed functions ----------//
		// Vertex input
		auto bindingDescription = GuiVertex::getBindingDescription();
		auto attributeDescriptions = GuiVertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		//vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;//static_cast<uint32_t>(attributeDescriptions.size());
		//vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		// Input assembly
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		// Viewport
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float) _imageExtent.width;
		viewport.height = (float) _imageExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = {0, 0};
		scissor.extent = _imageExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		// Rasterization
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		//rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_NONE;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		// Multisample
		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;//_device->getMsaaSamples();
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		// Color blend
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		// Depth stencil
		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f; // Optional
		depthStencil.maxDepthBounds = 1.0f; // Optional
		depthStencil.stencilTestEnable = VK_FALSE;
		
		//---------- Descriptors ----------//
		std::vector<DescriptorBinding> descriptorBindings =
		{
			{0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT},
			{1, static_cast<uint32_t>(guib::state::textures.size()+1), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		};

		_descriptorSetManager = std::make_shared<vk::DescriptorSetManager>(_device, descriptorBindings, 1);
		std::shared_ptr<vk::DescriptorSets> descriptorSets = _descriptorSetManager->getDescriptorSets();

		// Uniform buffer
		VkDescriptorBufferInfo uniformBufferInfo = {};
		uniformBufferInfo.buffer = uniformBuffer->handle();
		uniformBufferInfo.range = VK_WHOLE_SIZE;

		//---------- Bind descriptor sets ----------//
		// Image and texture samplers (font)
		std::vector<VkDescriptorImageInfo> imageInfos(1+guib::state::textures.size());

		// Load font texture
		imageInfos[0].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfos[0].imageView = _fontLoader->getTexture()->getImageView()->handle();
		imageInfos[0].sampler = _fontLoader->getTexture()->getSampler()->handle();
		// Font state textures (icons, use images, ...)
		for(size_t i=1;i<imageInfos.size(); i++)
		{
			imageInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfos[i].imageView = guib::state::textures[i-1]->getImageView()->handle();
			imageInfos[i].sampler = guib::state::textures[i-1]->getSampler()->handle();
		}

		const std::vector<VkWriteDescriptorSet> descriptorWrites =
		{
			descriptorSets->bind(0, 0, uniformBufferInfo),
			descriptorSets->bind(0, 1, *imageInfos.data(), static_cast<uint32_t>(imageInfos.size()))
		};

		descriptorSets->updateDescriptors(0, descriptorWrites);

		//---------- PipelineLayout ----------//
		_pipelineLayout = std::make_shared<GuiPipelineLayout>(_device, _descriptorSetManager->getDescriptorSetLayout());

		//---------- Create Pipeline ----------//
		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;

		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = nullptr; // Optional
		pipelineInfo.layout = _pipelineLayout->handle();
		pipelineInfo.renderPass = _renderPass->handle();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional	

		if(vkCreateGraphicsPipelines(_device->handle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS) 
		{
			Log::error("GuiPipeline", "Failed to create graphics pipeline!");
			exit(1);
		}
	}

	GuiPipeline::~GuiPipeline()
	{
		if(_pipeline != nullptr)
		{
			vkDestroyPipeline(_device->handle(), _pipeline, nullptr);
			_pipeline = nullptr;
		}
	}

	void GuiPipeline::beginRender(VkCommandBuffer commandBuffer, int imageIndex)
	{
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.0f, 0.0f, 0.0f, 0.0f};
		clearValues[1].depthStencil = {1.0f, 0};

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = _renderPass->handle();
		renderPassInfo.framebuffer = _frameBuffers[imageIndex]->handle();
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = _imageExtent;
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout->handle(), 0, 1, &_descriptorSetManager->getDescriptorSets()->handle()[0], 0, nullptr);
	}

	void GuiPipeline::endRender(VkCommandBuffer commandBuffer)
	{
		vkCmdEndRenderPass(commandBuffer);
	}
}
