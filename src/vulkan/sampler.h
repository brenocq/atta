#ifndef SAMPLER_H
#define SAMPLER_H

#include "device.h"

struct SamplerConfig final
{
	VkFilter magFilter = VK_FILTER_LINEAR;
	VkFilter minFilter = VK_FILTER_LINEAR;
	VkSamplerAddressMode addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	VkSamplerAddressMode addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	VkSamplerAddressMode addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	bool anisotropyEnable = true;
	float maxAnisotropy = 16;
	VkBorderColor borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	bool unnormalizedCoordinates = false;
	bool compareEnable = false;
	VkCompareOp compareOp = VK_COMPARE_OP_ALWAYS;
	VkSamplerMipmapMode mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	float mipLodBias = 0.0f;
	float minLod = 0.0f;
	float maxLod = 0.0f;
};

class Sampler
{
public:
	Sampler(Device* device, const SamplerConfig& config);
	~Sampler();

	Device* device() const { return _device; }
	VkSampler handle() const { return _sampler; }

private:

	Device* _device;
	VkSampler _sampler;
};

#endif// SAMPLER_H
