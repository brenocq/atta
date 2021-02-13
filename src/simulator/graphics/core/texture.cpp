//--------------------------------------------------
// Atta Graphics
// texture.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "texture.h"

namespace atta
{
	std::vector<Texture::TextureInfo> Texture::textureInfos = {};

	int Texture::fromBuffer(const void* data, unsigned width, unsigned height, BufferType bufferType)
	{
		TextureInfo info {
			.textureId = textureInfos.size(),
			.bufferType = bufferType,
			.data = data,
			.width = width,
			.height = height,
		};
		textureInfos.push_back(info);
		return info.textureId;
	}

	int Texture::updateTexture(int textureIndex)
	{
		TextureInfo info = textureInfos[textureIndex];

		if(auto texture = info.vkTexture.lock())
		{
			texture->updateImage(info.data, vk::Texture::BUFFER_RGBA);
		}
		else
		{
			Log::warning("Texture", "Texture not updated, must wait for the texture to be created before updating it.");
		}
	}
}
