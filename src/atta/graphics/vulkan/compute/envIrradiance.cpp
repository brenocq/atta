//--------------------------------------------------
// Atta Graphics
// envIrradiance.cpp
// Date: 2021-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/compute/envIrradiance.h>
#include <atta/graphics/vulkan/descriptorBinding.h>
#include <atta/graphics/vulkan/imageMemoryBarrier.h>

namespace atta::vk::compute
{
	EnvIrradiance::EnvIrradiance(CreateInfo info)
		: _device(info.device), _commandPool(info.commandPool), _input(info.input)
	{
		//---------- Create output image ----------//
		_output = std::make_shared<vk::Texture>(_device, _commandPool, 
				_input->getImage()->getExtent(), 
				_input->getImage()->getFormat(), 
				VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);

		//---------- Create shader module ----------//
		_shaderModule = std::make_shared<vk::ShaderModule>(_device, "/usr/include/atta/assets/shaders/compute/envIrradiance.comp.spv");
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shaderStageInfo.module = _shaderModule->handle();
		shaderStageInfo.pName = "main";

		//---------- Descriptors ----------//
		std::vector<DescriptorBinding> descriptorBindings =
		{
			// Input image (read-only)
			{0, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_COMPUTE_BIT},
			// Output image (write)
			{1, 1, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, VK_SHADER_STAGE_COMPUTE_BIT},
		};
		_descriptorSetManager = std::make_shared<vk::DescriptorSetManager>(_device, descriptorBindings, 1);
		std::shared_ptr<vk::DescriptorSets> descriptorSets = _descriptorSetManager->getDescriptorSets();

		// Image samplers
		VkDescriptorImageInfo inputImageInfo;
		inputImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		inputImageInfo.imageView = _input->getImageView()->handle();
		inputImageInfo.sampler = _input->getSampler()->handle();

		VkDescriptorImageInfo outputImageInfo;
		outputImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		outputImageInfo.imageView = _output->getImageView()->handle();
		outputImageInfo.sampler = _output->getSampler()->handle();

		const std::vector<VkWriteDescriptorSet> descriptorWrites =
		{
			descriptorSets->bind(0, 0, inputImageInfo),
			descriptorSets->bind(0, 1, outputImageInfo),
		};

		descriptorSets->updateDescriptors(0, descriptorWrites);

		//---------- Create pipeline layout ----------//
		// Descriptor set layouts
		VkDescriptorSetLayout setLayouts[] = { _descriptorSetManager->getDescriptorSetLayout()->handle() };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pSetLayouts = setLayouts;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if(vkCreatePipelineLayout(_device->handle(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
		{
			Log::error("atta::vk::compute::EnvIrradiance", "Failed to create pipeline layout!");
			exit(1);
		}

		//---------- Create compute pipeline ----------//
		VkComputePipelineCreateInfo pipelineInfo {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.layout = _pipelineLayout;
		pipelineInfo.flags = 0;
		pipelineInfo.stage = shaderStageInfo;

		if(vkCreateComputePipelines(_device->handle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS) 
		{
			Log::error("atta::vk::compute::EnvIrradiance", "Failed to create equirectangular to cubemap compute pipeline!");
			exit(1);
		}
	}

	EnvIrradiance::~EnvIrradiance()
	{
		if(_pipeline != nullptr)
		{
			vkDestroyPipeline(_device->handle(), _pipeline, nullptr);
			_pipeline = nullptr;
		}

		if(_pipelineLayout != nullptr)
		{
			vkDestroyPipelineLayout(_device->handle(), _pipelineLayout, nullptr);
			_pipelineLayout = nullptr;
		}
	}

	void EnvIrradiance::compute()
	{
		// Change image layouts
		VkCommandBuffer commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			VkImageSubresourceRange subresourceRange;
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.baseArrayLayer = 0;
			subresourceRange.layerCount = 1;

			ImageMemoryBarrier::insert(commandBuffer, _input->getImage()->handle(), subresourceRange, 0,
				VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
			ImageMemoryBarrier::insert(commandBuffer, _output->getImage()->handle(), subresourceRange, 0,
				VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);

		// Compute
		commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, _pipeline);
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, _pipelineLayout, 0, 1, &_descriptorSetManager->getDescriptorSets()->handle()[0], 0, 0);

			VkExtent2D extent = _input->getImage()->getExtent();
			vkCmdDispatch(commandBuffer, extent.width/16, extent.height/16, 1);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);

		// Change image layouts
		commandBuffer = _commandPool->beginSingleTimeCommands();
		{
			VkImageSubresourceRange subresourceRange;
			subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			subresourceRange.baseMipLevel = 0;
			subresourceRange.levelCount = 1;
			subresourceRange.baseArrayLayer = 0;
			subresourceRange.layerCount = 1;

			ImageMemoryBarrier::insert(commandBuffer, _input->getImage()->handle(), subresourceRange, 0,
				VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			ImageMemoryBarrier::insert(commandBuffer, _output->getImage()->handle(), subresourceRange, 0,
				VK_ACCESS_SHADER_WRITE_BIT, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}
		_commandPool->endSingleTimeCommands(commandBuffer);
	}
}
