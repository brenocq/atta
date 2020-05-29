#include "sampler.h"

Sampler::Sampler(Device* device, const SamplerConfig& config):
	_device(device)
{
	VkSamplerCreateInfo samplerInfo = {};
	samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	samplerInfo.magFilter = config.magFilter;
	samplerInfo.minFilter = config.minFilter;
	samplerInfo.addressModeU = config.addressModeU;
	samplerInfo.addressModeV = config.addressModeV;
	samplerInfo.addressModeW = config.addressModeW;
	samplerInfo.anisotropyEnable = config.anisotropyEnable;
	samplerInfo.maxAnisotropy = config.maxAnisotropy;
	samplerInfo.borderColor = config.borderColor;
	samplerInfo.unnormalizedCoordinates = config.unnormalizedCoordinates;
	samplerInfo.compareEnable = config.compareEnable;
	samplerInfo.compareOp = config.compareOp;
	samplerInfo.mipmapMode = config.mipmapMode;
	samplerInfo.mipLodBias = config.mipLodBias;
	samplerInfo.minLod = config.minLod;
	samplerInfo.maxLod = config.maxLod;

	if (vkCreateSampler(device->handle(), &samplerInfo, nullptr, &_sampler) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[Sampler] Failed to create sampler!";
		exit(1);
	}
}

Sampler::~Sampler()
{
	if (_sampler != nullptr)
		vkDestroySampler(_device->handle(), _sampler, nullptr);
}
