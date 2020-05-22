#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include <string>
#include <vector>
#include "device.h"

class ShaderModule
{
	public:
		ShaderModule(Device* device, const std::string& filename);
		ShaderModule(Device* device, const std::vector<char>& code);
		~ShaderModule();

		VkShaderModule handle() const { return _shaderModule; }
		Device* device() const { return _device; }

		VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits stage) const;

	private:
		static std::vector<char> readFile(const std::string& filename);
		Device* _device;
		VkShaderModule _shaderModule;
};

#endif// SHADER_MODULE_H
