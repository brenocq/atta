//--------------------------------------------------
// Robot Simulator
// texture.cpp
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "texture.h"
#include "stagingBuffer.h"

Texture::Texture(Device* device, string filename)
{
	_device = device;

	int texWidth, texHeight, texChannels;
	//"src/assets/textures/texture.jpg"
    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    VkDeviceSize imageSize = texWidth * texHeight * 4;

    if(!pixels) 
	{
		std::cout << BOLDRED << "[Texture]" << RESET << RED << " Failed to load texture image!" << RESET << std::endl;
		exit(1);
    }

	StagingBuffer* stagingBuffer = new StagingBuffer(_device, pixels, imageSize);
	delete stagingBuffer;
	stagingBuffer = nullptr;
	stbi_image_free(pixels);

	_image = new Image(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}

Texture::~Texture()
{
	if(_image != nullptr)
	{
		delete _image;
		_image = nullptr;
	}
}
