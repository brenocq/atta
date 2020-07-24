//--------------------------------------------------
// Robot Simulator
// rayTracingPipeline.cpp
// Date: 22/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "rayTracingPipeline.h"
#include "../descriptorBinding.h"
#include "../shaderModule.h"

RayTracingPipeline::RayTracingPipeline(
	Device* device,
	DeviceProcedures* deviceProcedures,
	SwapChain* swapChain,
	TopLevelAccelerationStructure& accelerationStructure,
	ImageView* accumulationImageView,
	ImageView* outputImageView,
	std::vector<UniformBuffer*> uniformBuffers,
	Scene* scene)
{
	_device = device;
	_swapChain = swapChain;
	_scene = scene;

	// Create descriptor pool/sets.
	const std::vector<DescriptorBinding> descriptorBindings =
	{
		// Top level acceleration structure.
		{0, 1, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, VK_SHADER_STAGE_RAYGEN_BIT_NV},

		// Image accumulation & output
		{1, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_RAYGEN_BIT_NV},
		{2, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_RAYGEN_BIT_NV},

		// Camera information & co
		{3, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_RAYGEN_BIT_NV | VK_SHADER_STAGE_MISS_BIT_NV},

		// Vertex buffer, Index buffer, Material buffer, Offset buffer
		{4, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV},
		{5, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV},
		{6, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV},
		{7, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV},

		// Textures and image samplers
		{8, static_cast<uint32_t>(scene->getTextures().size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV},

		// The Procedural buffer.
		{9, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV | VK_SHADER_STAGE_INTERSECTION_BIT_NV}
	};

	_descriptorSetManager = new DescriptorSetManager(device, descriptorBindings, uniformBuffers.size());

	DescriptorSets* descriptorSets = _descriptorSetManager->getDescriptorSets();

	for(uint32_t i = 0; i != _swapChain->getImages().size(); i++)
	{
		// Top level acceleration structure.
		const auto accelerationStructureHandle = accelerationStructure.handle();
		VkWriteDescriptorSetAccelerationStructureNV structureInfo = {};
		structureInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_NV;
		structureInfo.pNext = nullptr;
		structureInfo.accelerationStructureCount = 1;
		structureInfo.pAccelerationStructures = &accelerationStructureHandle;

		// Accumulation image
		VkDescriptorImageInfo accumulationImageInfo = {};
		accumulationImageInfo.imageView = accumulationImageView->handle();
		accumulationImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

		// Output image
		VkDescriptorImageInfo outputImageInfo = {};
		outputImageInfo.imageView = outputImageView->handle();
		outputImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

		// Uniform buffer
		VkDescriptorBufferInfo uniformBufferInfo = {};
		uniformBufferInfo.buffer = uniformBuffers[i]->handle();
		uniformBufferInfo.range = VK_WHOLE_SIZE;

		// Vertex buffer
		VkDescriptorBufferInfo vertexBufferInfo = {};
		vertexBufferInfo.buffer = _scene->getVertexBuffer()->handle();
		vertexBufferInfo.range = VK_WHOLE_SIZE;

		// Index buffer
		VkDescriptorBufferInfo indexBufferInfo = {};
		indexBufferInfo.buffer = _scene->getIndexBuffer()->handle();
		indexBufferInfo.range = VK_WHOLE_SIZE;

		// Material buffer
		VkDescriptorBufferInfo materialBufferInfo = {};
		materialBufferInfo.buffer = _scene->getMaterialBuffer()->handle();
		materialBufferInfo.range = VK_WHOLE_SIZE;

		// Offsets buffer
		VkDescriptorBufferInfo offsetsBufferInfo = {};
		offsetsBufferInfo.buffer = _scene->getOffsetBuffer()->handle();
		offsetsBufferInfo.range = VK_WHOLE_SIZE;

		// Image and texture samplers.
		std::vector<VkDescriptorImageInfo> imageInfos(_scene->getTextures().size());

		for(size_t t = 0; t != imageInfos.size(); t++)
		{
			auto& imageInfo = imageInfos[t];
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _scene->getTextures()[t]->getImageView()->handle();
			imageInfo.sampler = _scene->getTextures()[t]->getSampler()->handle();
		}

		std::vector<VkWriteDescriptorSet> descriptorWrites =
		{
			descriptorSets->bind(i, 0, structureInfo),
			descriptorSets->bind(i, 1, accumulationImageInfo),
			descriptorSets->bind(i, 2, outputImageInfo),
			descriptorSets->bind(i, 3, uniformBufferInfo),
			descriptorSets->bind(i, 4, vertexBufferInfo),
			descriptorSets->bind(i, 5, indexBufferInfo),
			descriptorSets->bind(i, 6, materialBufferInfo),
			descriptorSets->bind(i, 7, offsetsBufferInfo),
			descriptorSets->bind(i, 8, *imageInfos.data(), static_cast<uint32_t>(imageInfos.size()))
		};

		// Procedural buffer (optional)
		//VkDescriptorBufferInfo proceduralBufferInfo = {};
		//
		//if (scene.HasProcedurals())
		//{
		//	proceduralBufferInfo.buffer = scene.ProceduralBuffer().Handle();
		//	proceduralBufferInfo.range = VK_WHOLE_SIZE;

		//	descriptorWrites.push_back(descriptorSets.Bind(i, 9, proceduralBufferInfo));
		//}

		descriptorSets->updateDescriptors(i, descriptorWrites);
	}

	_pipelineLayout = new PipelineLayout(_device, _descriptorSetManager->getDescriptorSetLayout());

	// Load shaders.
	const ShaderModule rayGenShader(_device, "src/shaders/rayTracing/rayTracing.rgen.spv");
	const ShaderModule missShader(_device, "src/shaders/rayTracing/rayTracing.rmiss.spv");
	const ShaderModule closestHitShader(_device, "src/shaders/rayTracing/rayTracing.rchit.spv");
	const ShaderModule proceduralClosestHitShader(_device, "src/shaders/rayTracing/rayTracing.procedural.rchit.spv");
	const ShaderModule proceduralIntersectionShader(_device, "src/shaders/rayTracing/rayTracing.procedural.rint.spv");

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages =
	{
		rayGenShader.createShaderStage(VK_SHADER_STAGE_RAYGEN_BIT_NV),
		missShader.createShaderStage(VK_SHADER_STAGE_MISS_BIT_NV),
		closestHitShader.createShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV),
		proceduralClosestHitShader.createShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV),
		proceduralIntersectionShader.createShaderStage(VK_SHADER_STAGE_INTERSECTION_BIT_NV)
	};

	// Shader groups
	VkRayTracingShaderGroupCreateInfoNV rayGenGroupInfo = {};
	rayGenGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
	rayGenGroupInfo.pNext = nullptr;
	rayGenGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV;
	rayGenGroupInfo.generalShader = 0;
	rayGenGroupInfo.closestHitShader = VK_SHADER_UNUSED_NV;
	rayGenGroupInfo.anyHitShader = VK_SHADER_UNUSED_NV;
	rayGenGroupInfo.intersectionShader = VK_SHADER_UNUSED_NV;
	_rayGenIndex = 0;

	VkRayTracingShaderGroupCreateInfoNV missGroupInfo = {};
	missGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
	missGroupInfo.pNext = nullptr;
	missGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV;
	missGroupInfo.generalShader = 1;
	missGroupInfo.closestHitShader = VK_SHADER_UNUSED_NV;
	missGroupInfo.anyHitShader = VK_SHADER_UNUSED_NV;
	missGroupInfo.intersectionShader = VK_SHADER_UNUSED_NV;
	_missIndex = 1;

	VkRayTracingShaderGroupCreateInfoNV triangleHitGroupInfo = {};
	triangleHitGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
	triangleHitGroupInfo.pNext = nullptr;
	triangleHitGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV;
	triangleHitGroupInfo.generalShader = VK_SHADER_UNUSED_NV;
	triangleHitGroupInfo.closestHitShader = 2;
	triangleHitGroupInfo.anyHitShader = VK_SHADER_UNUSED_NV;
	triangleHitGroupInfo.intersectionShader = VK_SHADER_UNUSED_NV;
	_triangleHitGroupIndex = 2;

	VkRayTracingShaderGroupCreateInfoNV proceduralHitGroupInfo = {};
	proceduralHitGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
	proceduralHitGroupInfo.pNext = nullptr;
	proceduralHitGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_NV;
	proceduralHitGroupInfo.generalShader = VK_SHADER_UNUSED_NV;
	proceduralHitGroupInfo.closestHitShader = 3;
	proceduralHitGroupInfo.anyHitShader = VK_SHADER_UNUSED_NV;
	proceduralHitGroupInfo.intersectionShader = 4;
	_proceduralHitGroupIndex = 3;

	std::vector<VkRayTracingShaderGroupCreateInfoNV> groups =
	{
		rayGenGroupInfo, 
		missGroupInfo, 
		triangleHitGroupInfo, 
		proceduralHitGroupInfo,
	};

	// Create graphic pipeline
	VkRayTracingPipelineCreateInfoNV pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
	pipelineInfo.pNext = nullptr;
	pipelineInfo.flags = 0;
	pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.groupCount = static_cast<uint32_t>(groups.size());
	pipelineInfo.pGroups = groups.data();
	pipelineInfo.maxRecursionDepth = 1;
	pipelineInfo.layout = _pipelineLayout->handle();
	pipelineInfo.basePipelineHandle = nullptr;
	pipelineInfo.basePipelineIndex = 0;

	if(deviceProcedures->vkCreateRayTracingPipelinesNV(_device->handle(), nullptr, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
	{
		std::cerr << BOLDRED << "[RayTracingPipeline]" << RESET << RED << " Failed to create ray tracing pipeline!" << RESET << std::endl;
		exit(1);
	}
}

RayTracingPipeline::~RayTracingPipeline()
{
	if(_pipeline != nullptr)
	{
		vkDestroyPipeline(_device->handle(), _pipeline, nullptr);
		_pipeline = nullptr;
	}

	if(_pipelineLayout != nullptr)
	{
		delete _pipelineLayout;
		_pipelineLayout = nullptr;
	}

	if(_descriptorSetManager != nullptr)
	{
		delete _descriptorSetManager;
		_descriptorSetManager = nullptr;
	}
}

VkDescriptorSet RayTracingPipeline::getDescriptorSet(const uint32_t index) const
{
	return _descriptorSetManager->getDescriptorSets()->handle()[index];
}
