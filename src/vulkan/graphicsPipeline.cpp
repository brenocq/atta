#include "graphicsPipeline.h"
#include "buffer.h"
#include "descriptorPool.h"
#include "descriptorSets.h"
#include "device.h"
#include "shaderModule.h"
#include "../assets/vertex.h"
//#include "Assets/Scene.hpp"
//#include "Assets/UniformBuffer.hpp"
//#include "Assets/Vertex.hpp"

GraphicsPipeline::GraphicsPipeline(
	SwapChain* swapChain, 
	DepthBuffer* depthBuffer,
	//const std::vector<Assets::UniformBuffer>& uniformBuffers,
	//const Assets::Scene& scene,
	const bool isWireFrame) :
	_swapChain(swapChain),
	_isWireFrame(isWireFrame)
{
	Device* device = _swapChain->device();
	const auto bindingDescription = Vertex::getBindingDescription();
	const auto attributeDescriptions = Vertex::getAttributeDescriptions();

	//---- VERTEX INPUT ----// (TODO: Put in vertex descriptions when resources created)
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;//1;
	vertexInputInfo.pVertexBindingDescriptions = nullptr;//&bindingDescription;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;//static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = nullptr;//attributeDescriptions.data(); 
	
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(_swapChain->extent().width);
	viewport.height = static_cast<float>(_swapChain->extent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = _swapChain->extent();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = isWireFrame ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineDepthStencilStateCreateInfo depthStencil = {};
	depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencil.depthTestEnable = VK_TRUE;
	depthStencil.depthWriteEnable = VK_TRUE;
	depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.minDepthBounds = 0.0f; // Optional
	depthStencil.maxDepthBounds = 1.0f; // Optional
	depthStencil.stencilTestEnable = VK_FALSE;
	depthStencil.front = {}; // Optional
	depthStencil.back = {}; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	// Create descriptor pool/sets.
	std::vector<DescriptorBinding> descriptorBindings =
	{
		{0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT},
		{1, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
		//{2, static_cast<uint32_t>(scene.TextureSamplers().size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
	};

	//_descriptorSetManager = new DescriptorSetManager(device, descriptorBindings, uniformBuffers.size());

	//DescriptorSets* descriptorSets = _descriptorSetManager->descriptorSets();

	//for(uint32_t i = 0; i != _swapChain->images()->size(); i++)
	//{
	//	// Uniform buffer
	//	VkDescriptorBufferInfo uniformBufferInfo = {};
	//	uniformBufferInfo.buffer = uniformBuffers[i]->buffer()->handle();
	//	uniformBufferInfo.range = VK_WHOLE_SIZE;

	//	// Material buffer
	//	//VkDescriptorBufferInfo materialBufferInfo = {};
	//	//materialBufferInfo.buffer = scene->materialBuffer()->handle();
	//	//materialBufferInfo.range = VK_WHOLE_SIZE;

	//	// Image and texture samplers
	//	//std::vector<VkDescriptorImageInfo> imageInfos(scene.TextureSamplers().size());

	//	//for (size_t t = 0; t != imageInfos.size(); ++t)
	//	//{
	//	//	auto& imageInfo = imageInfos[t];
	//	//	imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	//	//	imageInfo.imageView = scene.TextureImageViews()[t];
	//	//	imageInfo.sampler = scene.TextureSamplers()[t];
	//	//}

	//	const std::vector<VkWriteDescriptorSet> descriptorWrites =
	//	{
	//		descriptorSets->bind(i, 0, uniformBufferInfo),
	//		//descriptorSets.Bind(i, 1, materialBufferInfo),
	//		//descriptorSets->bind(i, 2, *imageInfos.data(), static_cast<uint32_t>(imageInfos.size()))
	//	};

	//	descriptorSets->updateDescriptors(i, descriptorWrites);
	//}

	// Create pipeline layout and render pass.
	_pipelineLayout = new PipelineLayout(device/*, _descriptorSetManager->descriptorSetLayout()*/);
	_renderPass = new RenderPass(_swapChain, depthBuffer, true, true);

	// Load shaders.
	const ShaderModule vertShader(device, "src/shaders/vert.spv");
	const ShaderModule fragShader(device, "src/shaders/frag.spv");

	VkPipelineShaderStageCreateInfo shaderStages[] =
	{
		vertShader.createShaderStage(VK_SHADER_STAGE_VERTEX_BIT),
		fragShader.createShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT)
	};

	// Create graphic pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
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
	pipelineInfo.basePipelineHandle = nullptr; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional
	pipelineInfo.layout = _pipelineLayout->handle();
	pipelineInfo.renderPass = _renderPass->handle();
	pipelineInfo.subpass = 0;

	if(vkCreateGraphicsPipelines(device->handle(), nullptr, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[GraphicsPipeline] Failed to create graphics pipeline!" << RESET << std::endl;
		exit(1);
	}
}

GraphicsPipeline::~GraphicsPipeline()
{
	if(_pipeline != nullptr)
		vkDestroyPipeline(_swapChain->device()->handle(), _pipeline, nullptr);

	delete _renderPass;
	delete _pipelineLayout;
	//delete _descriptorSetManager;
}

VkDescriptorSet GraphicsPipeline::descriptorSet(const uint32_t index) const
{
	return _descriptorSetManager->descriptorSets()->handle(index);
}
