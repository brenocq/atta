//--------------------------------------------------
// Atta Graphics Module
// image.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/image.h>

namespace atta::graphics {

Image::Image(const CreateInfo& info)
    : _format(info.format), _samplerWrap(info.samplerWrap), _width(info.width), _height(info.height), _mipLevels(info.mipLevels),
      _isCubemap(info.isCubemap), _debugName(info.debugName), _data(info.data) {}

uint32_t Image::getNumChannels(Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
        case Format::RED32I:
        case Format::DEPTH32F:
            return 1;
        case Format::RG16F:
        case Format::DEPTH24_STENCIL8:
            return 2;
        case Format::RGB:
        case Format::BGR:
        case Format::RGB16F:
        case Format::RGB32F:
            return 3;
        case Format::RGBA:
        case Format::BGRA:
        case Format::RGBA32F:
            return 4;
    }
    ASSERT(false, "Could not calculate number of channels. Unknown image format");
    return 0;
}

uint32_t Image::getPixelSize(Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
            return 1;
        case Format::RGB:
        case Format::BGR:
            return 3;
        case Format::RGBA:
        case Format::BGRA:
        case Format::RED32I:
        case Format::RG16F:
        case Format::DEPTH32F:
        case Format::DEPTH24_STENCIL8:
            return 4;
        case Format::RGB16F:
            return 6;
        case Format::RGB32F:
            return 12;
        case Format::RGBA32F:
            return 16;
    }
    ASSERT(false, "Could not calculate pixel size. Unknown image format");
    return 0;
}

bool Image::isColorFormat(Format format) { return !isDepthFormat(format) && !isStencilFormat(format); }

bool Image::isDepthFormat(Format format) { return format == Format::DEPTH32F || format == Format::DEPTH24_STENCIL8; }

bool Image::isStencilFormat(Format format) { return format == Format::DEPTH24_STENCIL8; }

} // namespace atta::graphics
