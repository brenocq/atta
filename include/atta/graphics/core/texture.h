//--------------------------------------------------
// Atta Graphics
// texture.h
// Date: 2021-01-08
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_CORE_TEXTURE_H
#define ATTA_GRAPHICS_CORE_TEXTURE_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <atta/math/math.h>
#include <atta/graphics/vulkan/vulkan.h>

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
				FORMAT_RGBA_FLOAT,// 3 floats [0,1]
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

			static Texture& get()
			{
				static Texture texture;
				return texture;
			}

			// Create texture from file
			static int fromFile(std::string fileName) { return get().fromFileImpl(fileName); }
			// Create texture linked to a buffer
			static int fromBuffer(const void* data, unsigned width, unsigned height, Format format = FORMAT_RGBA_UBYTE) { return get().fromBufferImpl(data, width, height, format); }
			// Update texture from the buffer (copy from host memory to device memory)
			static void updateTexture(int textureIndex) { get().updateTextureImpl(textureIndex); }

			static VkFormat toVulkan(Format format) { return get()._toVulkan.at(format); }
			static size_t sizeInBytes(Format format) { return get()._sizeInBytes.at(format); }
			static std::vector<TextureInfo>& textureInfos() { return get()._textureInfos; }

		private:
			Texture(){};
			~Texture(){};
			Texture(const Texture&) = delete;
  			Texture& operator=(const Texture&) = delete;

			int fromFileImpl(std::string fileName);
			int fromBufferImpl(const void* data, unsigned width, unsigned height, Format format);
			void updateTextureImpl(int textureIndex);

			std::vector<TextureInfo> _textureInfos;

			const std::map<Format, VkFormat> _toVulkan = 
			{
				{FORMAT_R_UBYTE, VK_FORMAT_R8_UNORM},
				{FORMAT_RGBA_UBYTE, VK_FORMAT_R8G8B8A8_UNORM},
				{FORMAT_RGBA_FLOAT, VK_FORMAT_R32G32B32A32_SFLOAT}
			};

			const std::map<Format, size_t> _sizeInBytes =
			{
				{FORMAT_R_UBYTE, sizeof(uint8_t)},
				{FORMAT_RGBA_UBYTE, sizeof(uint8_t)*4},
				{FORMAT_RGBA_FLOAT, sizeof(float)*4}
			};
	};
}

#endif// ATTA_GRAPHICS_CORE_TEXTURE_H
