//--------------------------------------------------
// Atta Graphics System
// openGLImage.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLImage.h>

namespace atta
{
    OpenGLImage::OpenGLImage(const Image::CreateInfo& info):
        Image(info), _id(0)//, _samplerId(0)
    {
        // Create texture
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Populate data and generate mipmap
        GLenum dataType = OpenGLImage::convertDataType(_format);
        GLenum internalFormat = OpenGLImage::convertInternalFormat(_format);
        GLenum format = OpenGLImage::convertFormat(_format);
        if(info.data)
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, dataType, info.data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, dataType, 0);

        if(_format == Format::RGB || _format == Format::RGBA)
            glGenerateMipmap(GL_TEXTURE_2D);
    }

    OpenGLImage::~OpenGLImage()
    {
        if(_id)
            glDeleteTextures(1, &_id);
    }

    void OpenGLImage::write(void* data)
    {
        glBindTexture(GL_TEXTURE_2D, _id);
        GLenum dataType = OpenGLImage::convertDataType(_format);
        GLenum internalFormat = OpenGLImage::convertInternalFormat(_format);
        GLenum format = OpenGLImage::convertFormat(_format);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, format, dataType, data);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //------------------------------------------------//
    //---------- Atta to OpenGL conversions ----------//
    //------------------------------------------------//
    GLenum OpenGLImage::convertSizedInternalFormat(Format format)
    {
        switch(format)
        {
            case Format::NONE: break;
            case Format::RED: return GL_R8;
            case Format::RED32I: return GL_R32I;
            case Format::RGB: return GL_RGB8;
            case Format::RGBA: return GL_RGBA8;
            case Format::RGB16F: return GL_RGB16F;
            case Format::DEPTH32F: return GL_R32F;
            case Format::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
        }
        ASSERT(false, "Could not convert format to internal openGL format. Unknown image format");
    }

    GLenum OpenGLImage::convertFormat(Format format)
    {
        switch(format)
        {
            case Format::NONE: break;
            case Format::RED: return GL_RED;
            case Format::RED32I: return GL_RED_INTEGER;
            case Format::RGB16F:
            case Format::RGB: return GL_RGB;
            case Format::RGBA: return GL_RGBA;
            case Format::DEPTH32F: return GL_DEPTH_COMPONENT;
            case Format::DEPTH24_STENCIL8: return GL_DEPTH_STENCIL;
        }
        ASSERT(false, "Could not convert format to openGL format. Unknown image format");
    }

    GLenum OpenGLImage::convertInternalFormat(Format format)
    {
        switch(format)
        {
            case Format::NONE: break;
            case Format::RED:return GL_RED;
            case Format::RED32I: return GL_R32I;
            case Format::RGB16F: return GL_RGB16F;
            case Format::RGB: return GL_RGB;
            case Format::RGBA: return GL_RGBA;
            case Format::DEPTH32F: return GL_DEPTH_COMPONENT32F;
            case Format::DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
        }
        ASSERT(false, "Could not convert format to internal openGL format. Unknown image format");
    }

    GLenum OpenGLImage::convertDataType(Format format)
    {
        switch(format)
        {
            case Format::NONE: break;
            case Format::RED: 
            case Format::RGB: 
            case Format::RGBA: return GL_UNSIGNED_BYTE;
            case Format::RED32I: return GL_INT;
            case Format::RGB16F:
            case Format::DEPTH32F: return GL_FLOAT;
            case Format::DEPTH24_STENCIL8: return GL_UNSIGNED_INT_24_8;
        }
        ASSERT(false, "Could not convert format to openGL data type. Unknown image format");
    }

    GLenum OpenGLImage::convertSamplerWrap(Wrap samplerWrap)
    {
        switch(samplerWrap)
        {
            case Wrap::NONE: break;
            case Wrap::CLAMP: return GL_CLAMP_TO_EDGE;
            case Wrap::REPEAT: return GL_REPEAT;
        }
        ASSERT(false, "Could not convert sampler wrap to openGL sampler wrap. Unknown sampler wrap");
    }
}
