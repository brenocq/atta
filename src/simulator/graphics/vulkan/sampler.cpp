//--------------------------------------------------
// Robot Simulator
// sampler.cpp
// Date: 2020-07-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "sampler.h"

namespace atta::vk
{
	Sampler::Sampler(std::shared_ptr<Device> device, uint32_t mipLevels, bool unnormalizedCoordinates):
		_device(device)
	{

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR;

		samplerInfo.addressModeU = unnormalizedCoordinates?VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = unnormalizedCoordinates?VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = unnormalizedCoordinates?VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE:VK_SAMPLER_ADDRESS_MODE_REPEAT;

		samplerInfo.anisotropyEnable = unnormalizedCoordinates?VK_FALSE:VK_TRUE;
		samplerInfo.maxAnisotropy = 16.0f;

		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = false;//unnormalizedCoordinates; TODO Fix

		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;

		samplerInfo.mipmapMode = unnormalizedCoordinates?VK_SAMPLER_MIPMAP_MODE_NEAREST:VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.minLod = 0.0f; // Optional
		//samplerInfo.minLod = static_cast<float>(mipLevels / 2); // Test mipmap (low resolution)
		samplerInfo.maxLod = unnormalizedCoordinates?0:static_cast<float>(mipLevels);
		samplerInfo.mipLodBias = 0.0f; // Optional

		if(vkCreateSampler(_device->handle(), &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
		{
			std::cout << BOLDRED << "[Sampler]" << RESET << RED << " Failed to create texture sampler!" << RESET << std::endl;
			exit(1);
		}
	}

	Sampler::~Sampler()
	{
		if (_sampler != nullptr)
		{
			vkDestroySampler(_device->handle(), _sampler, nullptr);
			_sampler = nullptr;
		}
	}
}
