//--------------------------------------------------
// Atta Ray Tracing Vulkan
// rayTracingPipeline.cpp
// Date: 2020-07-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/renderers/rayTracing/rayTracingVulkan/rayTracingPipeline.h>
#include <atta/graphics/vulkan/descriptorBinding.h>
#include <atta/graphics/vulkan/descriptorSets.h>
#include <atta/graphics/vulkan/shaderModule.h>
#include <atta/helpers/log.h>
#include <atta/helpers/evaluator.h>
#include <future>// Used for deferred host operations when compiling

namespace atta::rt::vk
{
	RayTracingPipeline::RayTracingPipeline(
		std::shared_ptr<atta::vk::Device> device,
		std::shared_ptr<DeviceProcedures> deviceProcedures,
		std::shared_ptr<TopLevelAccelerationStructure> accelerationStructure,
		std::shared_ptr<atta::vk::ImageView> accumulationImageView,
		std::shared_ptr<atta::vk::ImageView> outputImageView,
		std::shared_ptr<rt::vk::UniformBuffer> uniformBuffer,
		std::shared_ptr<atta::vk::VulkanCore> vkCore):
		_device(device), _vkCore(vkCore)
	{
		//---------- Create descriptor set ----------//
		// Create descriptor pool/sets
		const std::vector<DescriptorBinding> descriptorBindings =
		{
			// Top level acceleration structure
			{0, 1, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR, VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},

			// Image accumulation & image output
			{1, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_RAYGEN_BIT_KHR},
			{2, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_RAYGEN_BIT_KHR},

			// Camera information
			{3, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_MISS_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},

			// Vertex buffer, Index buffer, Material buffer, Object info buffer
			{4, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},
			{5, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},
			{6, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},
			{7, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},

			// Light buffer
			{8, 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},

			// Textures and image samplers
			{9, static_cast<uint32_t>(_vkCore->getTextures().size()), VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 
				VK_SHADER_STAGE_RAYGEN_BIT_KHR | VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR},
		};

		_descriptorSetManager = std::make_shared<atta::vk::DescriptorSetManager>(device, descriptorBindings, 1);

		std::shared_ptr<atta::vk::DescriptorSets> descriptorSets = _descriptorSetManager->getDescriptorSets();

		// Top level acceleration structure.
		const auto accelerationStructureHandle = accelerationStructure->handle();
		VkWriteDescriptorSetAccelerationStructureKHR structureInfo = {};
		structureInfo.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_ACCELERATION_STRUCTURE_KHR;
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
		uniformBufferInfo.buffer = uniformBuffer->handle();
		uniformBufferInfo.range = VK_WHOLE_SIZE;

		// Vertex buffer
		VkDescriptorBufferInfo vertexBufferInfo = {};
		vertexBufferInfo.buffer = _vkCore->getVertexBuffer()->handle();
		vertexBufferInfo.range = VK_WHOLE_SIZE;

		// Index buffer
		VkDescriptorBufferInfo indexBufferInfo = {};
		indexBufferInfo.buffer = _vkCore->getIndexBuffer()->handle();
		indexBufferInfo.range = VK_WHOLE_SIZE;

		// Material buffer
		VkDescriptorBufferInfo materialBufferInfo = {};
		materialBufferInfo.buffer = _vkCore->getMaterialBuffer()->handle();
		materialBufferInfo.range = VK_WHOLE_SIZE;

		// Object info buffer
		VkDescriptorBufferInfo objInfoBufferInfo = {};
		objInfoBufferInfo.buffer = _vkCore->getObjectInfoBuffer()->handle();
		objInfoBufferInfo.range = VK_WHOLE_SIZE;

		// Light buffer
		VkDescriptorBufferInfo lightBufferInfo = {};
		lightBufferInfo.buffer = _vkCore->getLightBuffer()->handle();
		lightBufferInfo.range = VK_WHOLE_SIZE;

		// Image and texture samplers
		std::vector<VkDescriptorImageInfo> imageInfos(_vkCore->getTextures().size());
		for(size_t t = 0; t < imageInfos.size(); t++)
		{
			VkDescriptorImageInfo& imageInfo = imageInfos[t];
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = _vkCore->getTextures()[t]->getImageView()->handle();
			imageInfo.sampler = _vkCore->getTextures()[t]->getSampler()->handle();
		}

		const std::vector<VkWriteDescriptorSet> descriptorWrites =
		{
			descriptorSets->bind(0, 0, structureInfo),
			descriptorSets->bind(0, 1, accumulationImageInfo),
			descriptorSets->bind(0, 2, outputImageInfo),
			descriptorSets->bind(0, 3, uniformBufferInfo),
			descriptorSets->bind(0, 4, vertexBufferInfo),
			descriptorSets->bind(0, 5, indexBufferInfo),
			descriptorSets->bind(0, 6, materialBufferInfo),
			descriptorSets->bind(0, 7, objInfoBufferInfo),
			descriptorSets->bind(0, 8, lightBufferInfo),
			descriptorSets->bind(0, 9, *imageInfos.data(), static_cast<uint32_t>(imageInfos.size())),
		};

		// Procedural buffer (optional)
		VkDescriptorBufferInfo proceduralBufferInfo = {};
		
		//if(_scene->hasProcedurals())
		//{
		//	proceduralBufferInfo.buffer = _scene->getProceduralBuffer()->handle();
		//	proceduralBufferInfo.range = VK_WHOLE_SIZE;

		//	descriptorWrites.push_back(descriptorSets->bind(0, 10, proceduralBufferInfo));
		//}

		descriptorSets->updateDescriptors(0, descriptorWrites);

		_pipelineLayout = std::make_shared<atta::vk::PipelineLayout>(_device, _descriptorSetManager->getDescriptorSetLayout());

		//---------- Create shader binding table ----------//
		// Load shaders
		const atta::vk::ShaderModule rayGenShader(_device, "/usr/include/atta/assets/shaders/rayTracing/rayTracing.rgen.spv");
		const atta::vk::ShaderModule missShader(_device, "/usr/include/atta/assets/shaders/rayTracing/rayTracing.rmiss.spv");
		const atta::vk::ShaderModule missShadowShader(_device, "/usr/include/atta/assets/shaders/rayTracing/rayTracingShadow.rmiss.spv");
		const atta::vk::ShaderModule diffuseCHitShader(_device, "/usr/include/atta/assets/shaders/rayTracing/hitShaders/diffuse.rchit.spv");
		//const atta::vk::ShaderModule disneyCHitShader(_device, "/usr/include/atta/assets/shaders/rayTracing/hitShaders/disney.rchit.spv");

		std::vector<VkPipelineShaderStageCreateInfo> shaderStages =
		{
			rayGenShader.createShaderStage(VK_SHADER_STAGE_RAYGEN_BIT_KHR),
			missShader.createShaderStage(VK_SHADER_STAGE_MISS_BIT_KHR),
			missShadowShader.createShaderStage(VK_SHADER_STAGE_MISS_BIT_KHR),
			diffuseCHitShader.createShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR),
			//disneyCHitShader.createShaderStage(VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR),
		};

		// Shader groups
		VkRayTracingShaderGroupCreateInfoKHR rayGenGroupInfo = {};
		rayGenGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		rayGenGroupInfo.pNext = nullptr;
		rayGenGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
		rayGenGroupInfo.generalShader = _rayGenIndex = 0;
		rayGenGroupInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
		rayGenGroupInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
		rayGenGroupInfo.intersectionShader = VK_SHADER_UNUSED_KHR;

		VkRayTracingShaderGroupCreateInfoKHR missGroupInfo = {};
		missGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		missGroupInfo.pNext = nullptr;
		missGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
		missGroupInfo.generalShader = _missIndex = 1;
		missGroupInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
		missGroupInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
		missGroupInfo.intersectionShader = VK_SHADER_UNUSED_KHR;

		VkRayTracingShaderGroupCreateInfoKHR missShadowGroupInfo = {};
		missShadowGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		missShadowGroupInfo.pNext = nullptr;
		missShadowGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
		missShadowGroupInfo.generalShader = _missShadowIndex = 2;
		missShadowGroupInfo.closestHitShader = VK_SHADER_UNUSED_KHR;
		missShadowGroupInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
		missShadowGroupInfo.intersectionShader = VK_SHADER_UNUSED_KHR;

		VkRayTracingShaderGroupCreateInfoKHR diffuseHitGroupInfo = {};
		diffuseHitGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		diffuseHitGroupInfo.pNext = nullptr;
		diffuseHitGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
		diffuseHitGroupInfo.generalShader = VK_SHADER_UNUSED_KHR;
		diffuseHitGroupInfo.closestHitShader = _diffuseHitGroupIndex = 3;
		diffuseHitGroupInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
		diffuseHitGroupInfo.intersectionShader = VK_SHADER_UNUSED_KHR;

		//VkRayTracingShaderGroupCreateInfoKHR disneyHitGroupInfo = {};
		//disneyHitGroupInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
		//disneyHitGroupInfo.pNext = nullptr;
		//disneyHitGroupInfo.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
		//disneyHitGroupInfo.generalShader = VK_SHADER_UNUSED_KHR;
		//disneyHitGroupInfo.closestHitShader = _disneyHitGroupIndex = 4;
		//disneyHitGroupInfo.anyHitShader = VK_SHADER_UNUSED_KHR;
		//disneyHitGroupInfo.intersectionShader = VK_SHADER_UNUSED_KHR;

		std::vector<VkRayTracingShaderGroupCreateInfoKHR> groups =
		{
			rayGenGroupInfo, 
			missGroupInfo, 
			missShadowGroupInfo, 
			diffuseHitGroupInfo, 
			//disneyHitGroupInfo, 
		};

		// Create graphic pipeline
		VkRayTracingPipelineCreateInfoKHR pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
		pipelineInfo.pNext = nullptr;
		pipelineInfo.flags = 0;
		pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.groupCount = static_cast<uint32_t>(groups.size());
		pipelineInfo.pGroups = groups.data();
		pipelineInfo.maxPipelineRayRecursionDepth = 2;
		pipelineInfo.layout = _pipelineLayout->handle();
		pipelineInfo.basePipelineHandle = nullptr;
		pipelineInfo.basePipelineIndex = 0;

		// Create deferred operation to enable compiling with several threads
		VkDeferredOperationKHR deferredOperation;
  		if(deviceProcedures->vkCreateDeferredOperationKHR(_device->handle(), nullptr, &deferredOperation) != VK_SUCCESS)
		{
			Log::error("rt::vk::RayTracingPipeline", "Failed to create deferred operation!");
			exit(1);
		}

		// Create pipeline as deferred operation
		LocalEvaluator eval("vkCreateRayTracingPipelinesKHR");
		if(deviceProcedures->vkCreateRayTracingPipelinesKHR(_device->handle(), deferredOperation, nullptr, 1, &pipelineInfo, nullptr, &_pipeline) != VK_OPERATION_DEFERRED_KHR)
		{
			Log::error("rt::vk::RayTracingPipeline", "Failed to create ray tracing pipeline as deferred operation!");
			exit(1);
		}
		// TODO compile without deferred operation if it is not supported
		//if(deviceProcedures->vkCreateRayTracingPipelinesKHR(_device->handle(), nullptr, nullptr, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
		//{
		//	Log::error("rt::vk::RayTracingPipeline", "Failed to create ray tracing pipeline!");
		//	exit(1);
		//}

		// Execute threads
		uint32_t maxThreads{8};
  		uint32_t threadCount = std::min(deviceProcedures->vkGetDeferredOperationMaxConcurrencyKHR(_device->handle(), deferredOperation), maxThreads);
		Log::debug("rt::vk::RayTracingPipeline", "Compiling ray tracing using $0 threads", threadCount);
	 	std::vector<std::future<void>> joins;
	  	for(uint32_t i = 0; i < threadCount; i++)
	  	{
	  	  VkDevice device{_device->handle()};
	  	  joins.emplace_back(std::async(std::launch::async, [device, deferredOperation, deviceProcedures]() {
	  	    // Wait until the thread has finished its work
	  	    VkResult result = deviceProcedures->vkDeferredOperationJoinKHR(device, deferredOperation);
	  	    // A return value of SUCCESS means the pipeline compilation is done.
	  	    // THREAD_DONE indicates that thread has no work to do for this task
	  	    // (e.g. the operation could not be split into that many threads)
	  	    // THREAD_IDLE indicates the thread has finished its task, but the overall pipeline
	  	    // compilation is not finished.
	  	    // In the last two cases, more work could be performed by the thread, such as waiting
	  	    // for another deferred operation
	  	    if(!(result == VK_SUCCESS || result == VK_THREAD_DONE_KHR || result == VK_THREAD_IDLE_KHR))
			{
				Log::warning("rt::vk::RayTracingPipeline", "Thread strange result when compiling the pipeline: $0", (int)result);
			}
	  	  }));
	  	}
		// Wait for all threads to finish
		for(auto& f : joins)
		  f.get();

		// Once the deferred operation is complete, check for compilation success
		if(deviceProcedures->vkGetDeferredOperationResultKHR(_device->handle(), deferredOperation) != VK_SUCCESS)
		{
			Log::error("rt::vk::RayTracingPipeline", "Failed to finish ray tracing pipeline creating!");
			exit(1);
		}
		// Destroy the deferred operation
  		deviceProcedures->vkDestroyDeferredOperationKHR(_device->handle(), deferredOperation, nullptr);
	}

	RayTracingPipeline::~RayTracingPipeline()
	{
		if(_pipeline != nullptr)
		{
			vkDestroyPipeline(_device->handle(), _pipeline, nullptr);
			_pipeline = nullptr;
		}
	}

	VkDescriptorSet RayTracingPipeline::getDescriptorSet(const uint32_t index) const
	{
		return _descriptorSetManager->getDescriptorSets()->handle()[index];
	}
}
