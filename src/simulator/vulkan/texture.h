//--------------------------------------------------
// Robot Simulator
// texture.h
// Date: 07/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string.h>
#include "defines.h"
#include "device.h"
#include "image.h"

class Texture
{
	public:
	Texture(Device* device, string filename);
	~Texture();

	Device* getDevice() const { return _device; }
	Image* getImage() const { return _image; }

	private:

	Device* _device;
	Image* _image;
};

#endif// TEXTURE_H
