//--------------------------------------------------
// Atta Graphics
// texture.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_TEXTURE_H
#define ATTA_TEXTURE_H

#include <vector>
#include <string>
#include "simulator/math/math.h"

namespace atta
{
	// All textures that will be used in the simulation must be create using this class
	// After all objects are created in the project class, a vector of atta::vk::Texture is created from the atta::Texture::TextureInfo vector
	// and saved to the scene
	class Texture
	{
		public:
			enum BufferType {
				BUFFER_TYPE_NONE = 0,
				BUFFER_TYPE_VEC4,
			};

			static int fromBuffer(const void* data, unsigned width, unsigned height, BufferType bufferType = BUFFER_TYPE_VEC4);

		private:
			struct TextureInfo
			{
				int textureId;

				// Texture from buffer
				BufferType bufferType=BUFFER_TYPE_NONE;
				const void* data=nullptr;
				unsigned width=0;
				unsigned height=0;

				// Texture from file
				std::string fileName = "";
			};

			static std::vector<TextureInfo> _textureInfos;
	};
}

#endif// ATTA_TEXTURE_H
