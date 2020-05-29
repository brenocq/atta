#ifndef TEXTURE_H
#define TEXTURE_H

#include <memory>
#include <string>
#include "defines.h"
#include "../vulkan/stbImage.h"

class Texture final
{
public:

	static Texture loadTexture(const std::string& filename);

	Texture& operator = (const Texture&) = delete;
	Texture& operator = (Texture&&) = delete;

	Texture() = default;
	Texture(const Texture&) = default;
	Texture(Texture&&) = default;
	~Texture() = default;

	const unsigned char* pixels() const { return _pixels.get(); }
	int width() const { return _width; }
	int height() const { return _height; }

private:
	Texture(int width, int height, int channels, unsigned char* pixels);

	int _channels;
	int _width;
	int _height;
	std::unique_ptr<unsigned char, void (*) (void*)> _pixels;
};

#endif// TEXTURE_H
