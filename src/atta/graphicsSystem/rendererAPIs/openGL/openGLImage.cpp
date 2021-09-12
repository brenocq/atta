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
		Image(info), _id(0), _samplerId(0)
	{
		// Create texture
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		GLenum sizedFormat = OpenGLImage::convertSizedInternalFormat(_format);
		//glTextureStorage2D(_id, _mipLevels, sizedFormat, _width, _height);

		// Populate data and generate mipmap
		GLenum dataType = OpenGLImage::convertDataType(_format);
		GLenum internalFormat = OpenGLImage::convertInternalFormat(_format);
		if(info.data)
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, internalFormat, dataType, info.data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, _width, _height, 0, internalFormat, dataType, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glGenerateTextureMipmap(_id);

		// Create sampler
		//glCreateSamplers(1, &_samplerId);
		//glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_R, convertSamplerWrap(_samplerWrap));
		//glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, convertSamplerWrap(_samplerWrap));
		//glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, convertSamplerWrap(_samplerWrap));
	}

	OpenGLImage::~OpenGLImage()
	{
		if(_id)
			glDeleteTextures(1, &_id);
	}

	//------------------------------------------------//
	//---------- Atta to OpenGL conversions ----------//
	//------------------------------------------------//
	GLenum OpenGLImage::convertSizedInternalFormat(Format format)
	{
		switch(format)
		{
			case Format::NONE: break;
			case Format::RGB: return GL_RGB8;
			case Format::RGBA: return GL_RGBA8;
		}
		ASSERT(false, "Could not convert format to internal openGL format. Unknown image format");
	}

	GLenum OpenGLImage::convertInternalFormat(Format format)
	{
		switch(format)
		{
			case Format::NONE: break;
			case Format::RGB: return GL_RGB;
			case Format::RGBA: return GL_RGBA;
		}
		ASSERT(false, "Could not convert format to internal openGL format. Unknown image format");
	}

	GLenum OpenGLImage::convertDataType(Format format)
	{
		switch(format)
		{
			case Format::NONE: break;
			case Format::RGB: 
			case Format::RGBA: return GL_UNSIGNED_BYTE;
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
