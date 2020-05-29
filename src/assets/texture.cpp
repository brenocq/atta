#include "texture.h"
#include <chrono>
#include <iostream>

Texture Texture::loadTexture(const std::string& filename)
{
	std::cout << BOLDWHITE << "[Texture] " << RESET << "Loading [" << filename << "]... ";
	const auto timer = std::chrono::high_resolution_clock::now();

	// Load the texture in normal host memory.
	int width, height, channels;
	const auto pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);

	if (!pixels)
	{
		std::cout << BOLDRED << "ERROR" << RESET << std::endl;
		exit(1);
	}

	const auto elapsed = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - timer).count();

	std::cout << BOLDGREEN << "OK" << RESET << std::endl;
	std::cout << "\tInfo: (" << width << " x " << height << " x " << channels << ") " << std::endl;
	std::cout << "\tTime: " << elapsed << "s" << std::endl;

	return Texture(width, height, channels, pixels);
}

Texture::Texture(int width, int height, int channels, unsigned char* const pixels) :
	_width(width),
	_height(height),
	_channels(channels),
	_pixels(pixels, stbi_image_free)
{
}
