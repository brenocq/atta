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
#include <memory>
#include <map>
#include "simulator/math/math.h"
#include "simulator/graphics/vulkan/vulkan.h"

namespace atta
{
	// All textures that will be used in the simulation must be create using this class
	// After all objects are created in the project class, a vector of atta::vk::Texture is created from the atta::Texture::TextureInfo vector
	// and saved to the scene
	namespace vk{class Texture;}
	class Texture
	{
		public:
			enum Type {
				TYPE_NONE = 0,
				TYPE_FILE,
				TYPE_BUFFER,
			};

			enum Format {
				FORMAT_NONE=0,
				FORMAT_RGBA_UBYTE,// 4 unsigned bytes [0,255]
				FORMAT_R_UBYTE,// 1 unsigned byte [0,255]
			};

			struct TextureInfo
			{
				int textureId;
				Type type=TYPE_NONE;
				Format format=FORMAT_NONE;
				unsigned width=0;
				unsigned height=0;

				// Texture from file
				std::string fileName = "";

				// Texture from buffer
				const void* data=nullptr;

				// Pointer to the created vulkan texture
				std::weak_ptr<vk::Texture> vkTexture;
			};

			static const std::map<Format, VkFormat> toVulkan;
			static const std::map<Format, size_t> sizeInBytes;

			static Texture& get()
			{
				static Texture texture;
				return texture;
			}

			// Create texture from file
			static int fromFile(std::string fileName);
			// Create texture linked to a buffer
			static int fromBuffer(const void* data, unsigned width, unsigned height, Format format = FORMAT_RGBA_UBYTE);
			// Update texture from the buffer (copy from host memory to device memory)
			static int updateTexture(int textureIndex) { return updateTextureImpl(textureIndex); }

		private:
			Texture()
			{

			}

			static int fromFileImpl(std::string fileName);
			static int fromBufferImpl(const void* data, unsigned width, unsigned height, Format format);
			static int updateTextureImpl(int textureIndex);

			std::vector<TextureInfo> textureInfos;
	};
}

#endif// ATTA_TEXTURE_H
