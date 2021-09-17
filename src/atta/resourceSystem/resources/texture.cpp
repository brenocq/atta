//--------------------------------------------------
// Atta Resource System
// texture.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resources/texture.h>
#include <atta/fileSystem/fileManager.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace atta
{
	Texture::Texture(const fs::path& filename):
		Resource(filename)
	{
		loadTexture();
	}

	Texture::~Texture()
	{
		stbi_image_free(_data);
	}

	void Texture::loadTexture()
	{
		fs::path absolutePath = FileManager::solveResourcePath(_filename);

		int width, height, channels;
		_data = stbi_load(absolutePath.string().c_str(), &width, &height, &channels, 0); 

		if(_data)
		{
			_width = width;
			_height = height;
			_channels = channels;
			LOG_WARN("Texture", "$3 -> w:$0, h:$1, c:$2", _width, _height, _channels, absolutePath);
		}
		else
		{
			_width = 0;
			_height = 0;
			_channels = 0;
			LOG_ERROR("Texture", "Failed to load texture [w]$0[]", absolutePath);
		}
	}
}

