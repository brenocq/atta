//--------------------------------------------------
// Atta Graphics
// texture.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "texture.h"
#include "simulator/graphics/vulkan/texture.h"

namespace atta
{
	std::vector<Texture::TextureInfo> Texture::textureInfos = {};
	const std::map<Texture::Format, VkFormat> Texture::toVulkan = 
	{
		{FORMAT_R_UBYTE, VK_FORMAT_R8_UNORM},
		{FORMAT_RGBA_UBYTE, VK_FORMAT_R8G8B8A8_UNORM}
	};

	const std::map<Texture::Format, size_t> Texture::sizeInBytes =
	{
		{FORMAT_R_UBYTE, sizeof(uint8_t)},
		{FORMAT_RGBA_UBYTE, sizeof(uint8_t)*4}
	};

	int Texture::fromFile(std::string fileName)
	{
		TextureInfo info {
			.textureId = textureInfos.size(),
			.type = TYPE_FILE,
			.fileName = fileName
		};
		textureInfos.push_back(info);
		return info.textureId;
	}

	int Texture::fromBuffer(const void* data, unsigned width, unsigned height, Format format)
	{
		TextureInfo info {
			.textureId = textureInfos.size(),
			.type = TYPE_BUFFER,
			.format = format,
			.width = width,
			.height = height,
			.data = data,
		};
		textureInfos.push_back(info);
		return info.textureId;
	}

	int Texture::updateTexture(int textureIndex)
	{
		Log::debug("Texture", "ok");
		//TextureInfo info = textureInfos[textureIndex];

		//if(auto texture = info.vkTexture.lock())
		//{
		//	//texture->updateImage((void*)info.data);
		//}
		//else
		//{
		//	Log::warning("Texture", "Texture not updated, must wait for the texture to be created before updating it.");
		//}
	}

}
