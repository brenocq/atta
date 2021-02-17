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
#include "simulator/graphics/vulkan/texture.h"

namespace atta
{
	// All textures that will be used in the simulation must be create using this class
	// After all objects are created in the project class, a vector of atta::vk::Texture is created from the atta::Texture::TextureInfo vector
	// and saved to the scene
	class Texture
	{
		public:
			enum Type {
				TYPE_NONE = 0,
				TYPE_FILE,
				TYPE_BUFFER_BYTE_4,
				TYPE_BUFFER_FLOAT_3,
			};

			// Create texture from file
			static int fromFile(std::string fileName);
			// Create texture linked to a buffer
			static int fromBuffer(const void* data, unsigned width, unsigned height, Type type = TYPE_BUFFER_BYTE_4);
			// Update texture from the buffer (copy from host memory to device memory)
			static int updateTexture(int textureIndex);

			struct TextureInfo
			{
				int textureId;
				Type type=TYPE_NONE;

				// Texture from file
				std::string fileName = "";

				// Texture from buffer
				const void* data=nullptr;
				unsigned width=0;
				unsigned height=0;

				// Pointer to the created vulkan texture
				std::weak_ptr<vk::Texture> vkTexture;
			};

			static std::vector<TextureInfo> textureInfos;
	};
}

#endif// ATTA_TEXTURE_H
