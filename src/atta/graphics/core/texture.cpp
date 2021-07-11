//--------------------------------------------------
// Atta Graphics
// texture.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/core/texture.h>
#include <atta/graphics/vulkan/texture.h>
#include <atta/helpers/log.h>

namespace atta
{
	int Texture::fromFileImpl(std::string fileName)
	{
		TextureInfo info {
			.textureId = _textureInfos.size(),
			.type = TYPE_FILE,
			.fileName = fileName
		};
		_textureInfos.push_back(info);
		return info.textureId;
	}

	int Texture::fromBufferImpl(const void* data, unsigned width, unsigned height, Format format)
	{
		TextureInfo info {
			.textureId = _textureInfos.size(),
			.type = TYPE_BUFFER,
			.format = format,
			.width = width,
			.height = height,
			.data = data,
		};
		_textureInfos.push_back(info);
		return info.textureId;
	}

	void Texture::addTextureProcessImpl(size_t id, Process process)
	{
		if(id >= _textureInfos.size())
		{
			Log::warning("atta::Texture", "Invalid texture id to process");
			return;
		}

		TextureInfo& texInfoToProcess = _textureInfos[id];
		TextureInfo newInfo {
			.textureId = _textureInfos.size(),
			.type = TYPE_PROCESSED,
			.format = texInfoToProcess.format,
			.process = std::make_pair(process, id),
			.width = texInfoToProcess.width,
			.height = texInfoToProcess.height,
		};
		_textureInfos.push_back(newInfo);
	}

	void Texture::updateTextureImpl(int textureIndex)
	{
		TextureInfo info = _textureInfos[textureIndex];

		if(auto texture = info.vkTexture.lock())
		{
			texture->updateImage((void*)info.data);
		}
		else
		{
			Log::warning("Texture", "Texture not updated, must wait for the texture to be created before updating it.");
		}
	}

}
