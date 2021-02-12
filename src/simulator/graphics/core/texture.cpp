//--------------------------------------------------
// Atta Graphics
// texture.cpp
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "texture.h"

namespace atta
{
	std::vector<Texture::TextureInfo> Texture::_textureInfos = {};

	int Texture::fromBuffer(const void* data, unsigned width, unsigned height, BufferType bufferType)
	{
		TextureInfo info {
			.textureId = _textureInfos.size(),
			.bufferType = bufferType,
			.data = data,
			.width = width,
			.height = height,
		};
		_textureInfos.push_back(info);
		return info.textureId;
	}
}
