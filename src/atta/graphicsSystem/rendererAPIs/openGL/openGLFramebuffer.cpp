//--------------------------------------------------
// Atta Graphics System
// openGLFramebuffer.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLFramebuffer.h>
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLImage.h>

namespace atta
{
	OpenGLFramebuffer::OpenGLFramebuffer(const Framebuffer::CreateInfo& info):
		Framebuffer(info), _id(0), _depthAttachmentFormat(Image::Format::NONE)
	{
		// Populate color and depth attachments
		for(auto attachment : _attachments)
		{
			if(Image::isDepthFormat(attachment.format))
			{
				DASSERT(_depthAttachmentFormat == Image::Format::NONE, "It is not possible to create frameBuffer with more than one depth attachment");
				_depthAttachmentFormat = attachment.format;
			}
			else
				_colorAttachmentFormats.push_back(attachment.format);
		}

		resize(_width, _height, true);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{

	}

	void OpenGLFramebuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
		glViewport(0, 0, _width, _height);
	}

	void OpenGLFramebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height, bool forceRecreate)
	{
		if(!forceRecreate && width == _width && height == _height)
			return;

		_width = width;
		_height = height;

		if(_id)
		{
			glDeleteFramebuffers(1, &_id);
			_colorAttachments.clear();
		}

		glGenFramebuffers(1, &_id);
		glBindFramebuffer(GL_FRAMEBUFFER, _id);

		//----- Create color attachments -----//
		if(_colorAttachmentFormats.size() > 0)
		{
			_colorAttachments.resize(_colorAttachmentFormats.size());
			for(size_t i = 0; i < _colorAttachments.size(); i++)
				_colorAttachments[i] = createColorAttachment(_colorAttachmentFormats[i], static_cast<int>(i));
		}

		// Specify framebuffer color attachments
		if(_colorAttachments.size() > 0)
		{
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glNamedFramebufferDrawBuffers(_id, _colorAttachments.size(), buffers);
		}
		else
			glNamedFramebufferDrawBuffer(_id, GL_NONE);

		//----- Create depth attachment -----//
		if(_depthAttachmentFormat != Image::Format::NONE)
		{
			_depthAttachment = createDepthAttachment(_depthAttachmentFormat);
		}

		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, 
				"Framebuffer is incomplete and can not be created ($0)", glCheckFramebufferStatus(GL_FRAMEBUFFER));

		// Unbind framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	//------------------------------------------------//
	//------------- Attachment creation --------------//
	//------------------------------------------------//
	std::shared_ptr<Image> OpenGLFramebuffer::createColorAttachment(Image::Format format, int index)
	{
		std::shared_ptr<OpenGLImage> image;
		Image::CreateInfo info;
		info.format = format;
		info.width = _width;
		info.height = _height;
		image = std::make_shared<OpenGLImage>(info);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, image->getId(), 0);
		LOG_DEBUG("OpenGLFramebuffer", "Created color attachment $0", index);

		return std::static_pointer_cast<Image>(image);
	}

	std::shared_ptr<Image> OpenGLFramebuffer::createDepthAttachment(Image::Format format)
	{
		std::shared_ptr<OpenGLImage> image;
		Image::CreateInfo info;
		info.format = format;
		info.width = _width;
		info.height = _height;
		image = std::make_shared<OpenGLImage>(info);

		glFramebufferTexture2D(GL_FRAMEBUFFER, convertDepthAttachmentType(format), GL_TEXTURE_2D, image->getId(), 0);

		return std::static_pointer_cast<Image>(image);
	}

	//------------------------------------------------//
	//---------- Atta to OpenGL conversions ----------//
	//------------------------------------------------//
	GLenum OpenGLFramebuffer::convertDepthAttachmentType(Image::Format format)
	{
		switch(format)
		{
			case Image::Format::NONE: break;
			case Image::Format::DEPTH32F: return GL_DEPTH_ATTACHMENT;
		}
		ASSERT(false, "Could not convert format to openGL depth attachment");
	}
}
