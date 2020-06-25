//--------------------------------------------------
// Robot Simulator
// shaderModule.h
// Date: 24/06/2020
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
		ShaderModule(Device* device, const std::string& filename);
		~ShaderModule();

		VkShaderModule handle() const { return _shaderModule; }

		//VkPipelineShaderStageCreateInfo createShaderStage(VkShaderStageFlagBits stage) const;

	private:
		std::vector<char> readFile(const std::string& filename);

		Device* _device;
		VkShaderModule _shaderModule;
		std::vector<char> _code;
};

#endif// SHADER_MODULE_H
