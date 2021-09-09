//--------------------------------------------------
// Atta Graphics System
// image.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/image.h>

namespace atta
{
	Image::Image(const CreateInfo& info):
		_format(info.format), _samplerWrap(info.samplerWrap), 
		_width(info.width), _height(info.height), _mipLevels(info.mipLevels),
		_debugName(info.debugName)
	{

	}

	uint32_t Image::getFormatSize(Format format)
	{
		switch(format)
		{
			case Format::NONE: break;
			case Format::RGB: return 3;
			case Format::RGBA: return 4;
		}
		ASSERT(false, "Could not calculate format size. Unknown image format");
	}

	bool Image::isDepthFormat(Format format)
	{
		return format == Format::DEPTH32F;
	}
}
