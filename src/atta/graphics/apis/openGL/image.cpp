//--------------------------------------------------
// Atta Graphics Module
// image.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/image.h>

namespace atta::graphics::gl {

Image::Image(const Image::CreateInfo& info) : gfx::Image(info), _id(0) { resize(_width, _height, true); }

Image::~Image() {
    if (_id) {
        glDeleteTextures(1, &_id);
    }
}

void Image::write(uint8_t* data) {
    if (!_isCubemap) {
        // LOG_DEBUG("graphics::OpenGlImage", "Writing $0 -> $1 ($2)", data, (int)_id, _debugName);
        GLenum dataType = Image::convertDataType(_format);
        GLenum internalFormat = Image::convertInternalFormat(_format);
        GLenum format = Image::convertFormat(_format);

        glBindTexture(GL_TEXTURE_2D, _id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, format, dataType, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    } else
        LOG_WARN("gfx::gl::Image", "Writing to cubemap image is not implemented yet. Image debug name: [w]$0[]", _debugName);
}

std::vector<uint8_t> Image::read(vec2i offset, vec2i size) {
    if (offset == vec2i() && size == vec2i())
        size = vec2i(_width, _height);
    if (_framebufferRead)
        return _framebufferRead(offset, size);
    LOG_DEBUG("gfx::gl::Image", "Can not read from an image that is not a framebuffer attachment");
    return {};
}

void Image::resize(uint32_t width, uint32_t height, bool forceRecreate) {
    // Check if size was not changed
    if (!forceRecreate && (_width == width && _height == height))
        return;
    _width = width;
    _height = height;

    if (_id)
        glDeleteTextures(1, &_id);

    unsigned test;
    glGenTextures(1, &test);
    _id = test;

    // Populate data and generate mipmap
    GLenum dataType = Image::convertDataType(_format);
    GLenum internalFormat = Image::convertInternalFormat(_format);
    GLenum format = Image::convertFormat(_format);
    GLint wrapMode = convertSamplerWrap(_samplerWrap);

    if (!_isCubemap) {
        // Create 2D texture
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (_samplerWrap == Image::Wrap::BORDER) {
            float borderColor[] = {_borderColor.x, _borderColor.y, _borderColor.z, _borderColor.w};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, dataType, _data);

        if (_mipLevels > 1)
            glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        // Create cubemap
        glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

        for (unsigned int i = 0; i < 6; i++)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, _width, _height, 0, format, dataType, _data);
        // glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, wrapMode);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (_samplerWrap == Image::Wrap::BORDER) {
            float borderColor[] = {_borderColor.x, _borderColor.y, _borderColor.z, _borderColor.w};
            glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        if (_mipLevels > 1)
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }
}

void Image::setFramebufferRead(std::function<std::vector<uint8_t>(vec2i, vec2i)> framebufferRead) { _framebufferRead = framebufferRead; }

//------------------------------------------------//
//---------- Atta to OpenGL conversions ----------//
//------------------------------------------------//
GLenum Image::convertSizedInternalFormat(Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
            return GL_R8;
        case Format::RED32I:
            return GL_R32I;
        case Format::RG16F:
            return GL_RG16F;
        case Format::RGB:
            return GL_RGB8;
        case Format::RGBA:
            return GL_RGBA8;
        case Format::RGB16F:
            return GL_RGB16F;
        case Format::RGB32F:
            return GL_RGB32F;
        case Format::RGBA32F:
            return GL_RGBA32F;
        case Format::DEPTH32F:
            return GL_R32F;
        case Format::DEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
    }
    ASSERT(false, "Could not convert format to internal openGL format. Unknown image format");
}

GLenum Image::convertFormat(Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
            return GL_RED;
        case Format::RED32I:
            return GL_RED_INTEGER;
        case Format::RG16F:
            return GL_RG;
        case Format::RGB:
        case Format::RGB16F:
        case Format::RGB32F:
            return GL_RGB;
        case Format::RGBA:
        case Format::RGBA32F:
            return GL_RGBA;
        case Format::DEPTH32F:
            return GL_DEPTH_COMPONENT;
        case Format::DEPTH24_STENCIL8:
            return GL_DEPTH_STENCIL;
    }
    ASSERT(false, "Could not convert format to openGL format. Unknown image format");
}

GLenum Image::convertInternalFormat(Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
            return GL_RED;
        case Format::RG16F:
            return GL_RG16F;
        case Format::RED32I:
            return GL_R32I;
        case Format::RGB16F:
            return GL_RGB16F;
        case Format::RGB32F:
            return GL_RGB32F;
        case Format::RGB:
            return GL_RGB;
        case Format::RGBA:
            return GL_RGBA;
        case Format::RGBA32F:
            return GL_RGBA32F;
        case Format::DEPTH32F:
            return GL_DEPTH_COMPONENT32F;
        case Format::DEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
    }
    ASSERT(false, "Could not convert format to internal openGL format. Unknown image format");
}

GLenum Image::convertDataType(Format format) {
    switch (format) {
        case Format::NONE:
            break;
        case Format::RED:
        case Format::RGB:
        case Format::RGBA:
            return GL_UNSIGNED_BYTE;
        case Format::RED32I:
            return GL_INT;
        case Format::RG16F:
        case Format::RGB16F:
        case Format::RGB32F:
        case Format::RGBA32F:
        case Format::DEPTH32F:
            return GL_FLOAT;
        case Format::DEPTH24_STENCIL8:
            return GL_UNSIGNED_INT_24_8;
    }
    ASSERT(false, "Could not convert format to openGL data type. Unknown image format");
}

GLenum Image::convertSamplerWrap(Wrap samplerWrap) {
    switch (samplerWrap) {
        case Wrap::NONE:
            break;
        case Wrap::CLAMP:
            return GL_CLAMP_TO_EDGE;
        case Wrap::REPEAT:
            return GL_REPEAT;
        case Wrap::BORDER:
            return GL_CLAMP_TO_BORDER;
    }
    ASSERT(false, "Could not convert sampler wrap to openGL sampler wrap. Unknown sampler wrap");
}

} // namespace atta::graphics::gl
