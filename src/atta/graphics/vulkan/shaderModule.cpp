//--------------------------------------------------
// Robot Simulator
// shaderModule.cpp
// Date: 2020-06-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/vulkan/shaderModule.h>
#include <atta/helpers/log.h>

namespace atta::vk
{
	ShaderModule::ShaderModule(std::shared_ptr<Device> device, const std::string& filename):
		_device(device)
	{
		_code = readFile(filename);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = _code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(_code.data());

		if(vkCreateShaderModule(_device->handle(), &createInfo, nullptr, &_shaderModule) != VK_SUCCESS) 
		{
			Log::error("ShaderModule", "Failed to create shader module!");
			exit(1);
		}
		else
		{
			//Log::success("ShaderModule", "Shader module with handle $0 created from file $1!", (long)_shaderModule, filename);
		}
	}

	ShaderModule::~ShaderModule()
	{
		if(_shaderModule != nullptr)
		{
			vkDestroyShaderModule(_device->handle(), _shaderModule, nullptr);
			_shaderModule = nullptr;
		}
	}

	std::vector<char> ShaderModule::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if(!file.is_open()) 
		{
			Log::error("ShaderModule", "Failed to open file: [w]$0[]!", filename);
			exit(1);
		}

		size_t fileSize = (size_t) file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
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
}
