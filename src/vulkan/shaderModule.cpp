#include "shaderModule.h"
#include "device.h"
#include <fstream>

ShaderModule::ShaderModule(Device* device, const std::string& filename):
	ShaderModule(device, readFile(filename))
{
	
}

ShaderModule::ShaderModule(Device* device, const std::vector<char>& code):
	_device(device)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	if(vkCreateShaderModule(device->handle(), &createInfo, nullptr, &_shaderModule) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[ShaderModule] Failed to create shader module!" << RESET << std::endl;
		exit(1);
	}
}

ShaderModule::~ShaderModule()
{
	if (_shaderModule != nullptr)
	{
		vkDestroyShaderModule(_device->handle(), _shaderModule, nullptr);
		_shaderModule = nullptr;
	}
}

VkPipelineShaderStageCreateInfo ShaderModule::createShaderStage(VkShaderStageFlagBits stage) const
{
	VkPipelineShaderStageCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	createInfo.stage = stage;
	createInfo.module = _shaderModule;
	createInfo.pName = "main";

	return createInfo;
}

std::vector<char> ShaderModule::readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		std::cout << BOLDRED << "[ShaderModule] Failed to open file: " << RED << filename << RESET << std::endl;
		exit(1);
	}

	const auto fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();

	return buffer;
}
