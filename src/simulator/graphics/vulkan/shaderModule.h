//--------------------------------------------------
// Robot Simulator
// shaderModule.h
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef SHADER_MODULE_H
#define SHADER_MODULE_H

#include <string>
#include <vector>
#include <fstream>
#include "device.h"

class ShaderModule
{
	public:
		ShaderModule(std::shared_ptr<Device> device, const std::string& filename);
		~ShaderModule();

		VkShaderModule handle() const { return _shaderModule; }

		VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits stage) const;

	private:
		std::vector<char> readFile(const std::string& filename);

		std::shared_ptr<Device> _device;
		VkShaderModule _shaderModule;
		std::vector<char> _code;
};

#endif// SHADER_MODULE_H
