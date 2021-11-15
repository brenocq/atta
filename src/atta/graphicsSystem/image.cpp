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
            case Format::RED: return 1;
            case Format::RGB: return 3;
            case Format::RGBA: return 4;
            case Format::RED32I: return 4;
            case Format::RGB16F: return 3;
            default: break;
        }
        ASSERT(false, "Could not calculate format size. Unknown image format");
    }

    bool Image::isDepthFormat(Format format)
    {
        return format == Format::DEPTH32F || format == Format::DEPTH24_STENCIL8;
    }

    bool Image::isStencilFormat(Format format)
    {
        return format == Format::DEPTH24_STENCIL8;
    }
}
