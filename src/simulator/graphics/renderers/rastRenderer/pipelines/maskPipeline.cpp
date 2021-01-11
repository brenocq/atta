//--------------------------------------------------
// Robot Simulator
// maskPipeline.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "maskPipeline.h"

#include "simulator/objects/basics/basics.h"
#include "simulator/objects/others/display/display.h"
#include "simulator/objects/sensors/camera/camera.h"
#include "simulator/physics/physicsEngine.h"
#include "simulator/helpers/log.h"

namespace atta::vk
{
	// Mask Pipeline with swapchain
	MaskPipeline::MaskPipeline(std::shared_ptr<Device> device, std::shared_ptr<SwapChain> swapChain, RenderPass* renderPass, std::vector<UniformBuffer*> uniformBuffers, Scene* scene):
		MaskPipeline(device, renderPass, swapChain->getExtent(), swapChain->getImageFormat(), swapChain->getImageViews(), uniformBuffers, scene)
	{
	}

	// Offline Mask Pipeline
	MaskPipeline::MaskPipeline(
			std::shared_ptr<Device> device, 
			RenderPass* renderPass,
			VkExtent2D extent, VkFormat format,
			ImageView* imageView,
			UniformBuffer* uniformBuffer, 
			Scene* scene):
		MaskPipeline(device, renderPass, extent, format, (std::vector<ImageView*>){imageView}, (std::vector<UniformBuffer*>){uniformBuffer}, scene)
	{
	}

	// Base constructor
	MaskPipeline::MaskPipeline(
			std::shared_ptr<Device> device, 
			RenderPass* renderPass,
			VkExtent2D extent, VkFormat format,
			std::vector<ImageView*> imageViews, 
			std::vector<UniformBuffer*> uniformBuffers, 
			Scene* scene):
		Pipeline(device, imageViews, scene), _imageExtent(extent), _imageFormat(format)
	{
		//---------- Render pass ----------//
		_renderPass = renderPass;

		//---------- Shaders ----------//
		_vertShaderModule = new ShaderModule(_device, "src/shaders/shaders/maskShader.vert.spv");
		_fragShaderModule = new ShaderModule(_device, "src/shaders/shaders/maskShader.frag.spv");

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
		auto bindingDescription = Vertex::getBindingDescription();
		auto attributeDescriptions = Vertex::getAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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
		//rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
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
		multisampling.rasterizationSamples = _device->getMsaaSamples();
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
		depthStencil.depthTestEnable = VK_FALSE;
		depthStencil.depthWriteEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_TRUE;
		depthStencil.back.compareOp = VK_COMPARE_OP_ALWAYS;
		depthStencil.back.failOp = VK_STENCIL_OP_REPLACE;
		depthStencil.back.depthFailOp = VK_STENCIL_OP_REPLACE;
		depthStencil.back.passOp = VK_STENCIL_OP_REPLACE;
		depthStencil.back.compareMask = 0xff;
		depthStencil.back.writeMask = 0xff;
		depthStencil.back.reference = 1;
		depthStencil.front = depthStencil.back;

		//---------- Descriptors ----------//
		std::vector<DescriptorBinding> descriptorBindings =
		{
			{0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
			{1, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
			{2, static_cast<uint32_t>(scene->getTextures().size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
		};

		_descriptorSetManager = new DescriptorSetManager(_device, descriptorBindings, uniformBuffers.size());
		DescriptorSets* descriptorSets = _descriptorSetManager->getDescriptorSets();

		for(uint32_t i = 0; i < uniformBuffers.size(); i++)
		{
			// Uniform buffer
			VkDescriptorBufferInfo uniformBufferInfo = {};
			uniformBufferInfo.buffer = uniformBuffers[i]->handle();
			uniformBufferInfo.range = VK_WHOLE_SIZE;

			// Material buffer
			VkDescriptorBufferInfo materialBufferInfo = {};
			materialBufferInfo.buffer = _scene->getMaterialBuffer()->handle();
			materialBufferInfo.range = VK_WHOLE_SIZE;

			// Image and texture samplers
			std::vector<VkDescriptorImageInfo> imageInfos(_scene->getTextures().size());

			for (size_t t = 0; t != imageInfos.size(); ++t)
			{
				auto& imageInfo = imageInfos[t];
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = _scene->getTextures()[t]->getImageView()->handle();
				imageInfo.sampler = _scene->getTextures()[t]->getSampler()->handle();
			}

			const std::vector<VkWriteDescriptorSet> descriptorWrites =
			{
				descriptorSets->bind(i, 0, uniformBufferInfo),
				descriptorSets->bind(i, 1, materialBufferInfo),
				descriptorSets->bind(i, 2, *imageInfos.data(), static_cast<uint32_t>(imageInfos.size()))
			};

			descriptorSets->updateDescriptors(i, descriptorWrites);
		}

		//---------- PipelineLayout ----------//
		_pipelineLayout = new PipelineLayout(_device, _descriptorSetManager->getDescriptorSetLayout());

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
			std::cout << BOLDRED << "[MaskPipeline]" << RESET << RED << " Failed to create mask pipeline!" << RESET << std::endl;
			exit(1);
		}
	}


	MaskPipeline::~MaskPipeline()
	{
	}

	void MaskPipeline::render(VkCommandBuffer commandBuffer, int imageIndex)
	{
		VkBuffer vertexBuffers[] = { _scene->getVertexBuffer()->handle() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout->handle(), 0, 1, &_descriptorSetManager->getDescriptorSets()->handle()[imageIndex], 0, nullptr);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffer, _scene->getIndexBuffer()->handle(), 0, VK_INDEX_TYPE_UINT32);

		for(auto abstractPtr : _scene->getObjects())
		{
			if(abstractPtr->getSelection()!=Object::ObjectSelection::SELECTED)
				continue;

			Model* model;
			if(abstractPtr->getType() == "ImportedObject")
				model = ((ImportedObject*)abstractPtr)->getModel();
			else if(abstractPtr->getType() == "Plane")
				model = ((Plane*)abstractPtr)->getModel();
			else if(abstractPtr->getType() == "Box")
				model = ((Box*)abstractPtr)->getModel();
			else if(abstractPtr->getType() == "Sphere")
				model = ((Sphere*)abstractPtr)->getModel();
			else if(abstractPtr->getType() == "Cylinder")
				model = ((Cylinder*)abstractPtr)->getModel();
			else if(abstractPtr->getType() == "Display")
				model = ((Display*)abstractPtr)->getModel();
			else continue;

			if(model == nullptr)
				continue;

			ObjectInfo objectInfo;
			// Model matrix
			objectInfo.modelMatrix = abstractPtr->getModelMat();

			// Color
			if(abstractPtr->getType() == "Plane")
				objectInfo.color = ((Plane*)abstractPtr)->getColor();
			else if(abstractPtr->getType() == "Box")
				objectInfo.color = ((Box*)abstractPtr)->getColor();
			else if(abstractPtr->getType() == "Sphere")
				objectInfo.color = ((Sphere*)abstractPtr)->getColor();
			else if(abstractPtr->getType() == "Cylinder")
				objectInfo.color = ((Cylinder*)abstractPtr)->getColor();
			else 
				objectInfo.color = {1,1,1};

			// Material index
			if(abstractPtr->getType() == "Display")
				objectInfo.materialIndex = ((Display*)abstractPtr)->getMaterialIndex();

			vkCmdPushConstants(
					commandBuffer,
					_pipelineLayout->handle(),
					VK_SHADER_STAGE_VERTEX_BIT,
					0,
					sizeof(ObjectInfo),
					&objectInfo);

			//const uint32_t vertexCount = model->getVerticesSize();
			const uint32_t indexCount = model->getIndicesSize();
			const uint32_t vertexOffset = model->getVertexOffset();
			const uint32_t indexOffset = model->getIndexOffset();

			vkCmdDrawIndexed(commandBuffer, indexCount, 1, indexOffset, vertexOffset, 0);
		}
	}
}
