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
        Framebuffer(info), _id(0), _depthAttachmentIndex(-1), _stencilAttachmentIndex(-1)
    {
        // Populate color and depth attachments
        int i = 0;
        for(auto attachment : _attachments)
        {
            if(Image::isDepthFormat(attachment.format))
            {
                DASSERT(_depthAttachmentIndex == -1, "It is not possible to create frameBuffer with more than one depth attachment");
                _depthAttachmentIndex = i;
            }

            if(Image::isStencilFormat(attachment.format))
            {
                DASSERT(_stencilAttachmentIndex == -1, "It is not possible to create frameBuffer with more than one stencil attachment");
                _stencilAttachmentIndex = i;
            }
            i++;
        }

        resize(_width, _height, true);
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        if(_id)
        {
            glDeleteFramebuffers(1, &_id);
        }
    }

    void OpenGLFramebuffer::bind(bool clear)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glViewport(0, 0, _width, _height);
        if(clear)
        {
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            if(_depthAttachmentIndex != -1 && _stencilAttachmentIndex != -1)
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);
                glEnable(GL_STENCIL_TEST);
                glStencilMask(0x00);
            }
            else if(_depthAttachmentIndex != -1)
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glEnable(GL_DEPTH_TEST);
            }
            else
                glClear(GL_COLOR_BUFFER_BIT);
        }
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
            _images.clear();
        }

        glGenFramebuffers(1, &_id);
        glBindFramebuffer(GL_FRAMEBUFFER, _id);

        //----- Create attachment images -----//
        for(unsigned i = 0; i < _attachments.size(); i++)
        {
            if(i == (unsigned)_depthAttachmentIndex)
                _images[i] = createDepthAttachment(_attachments[i].format);
            else
                _images[i] = createColorAttachment(_attachments[i].format, static_cast<int>(i));
        }

        //----- Check framebuffer-----//
        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, 
                "Framebuffer is incomplete and can not be created ($0)", glCheckFramebufferStatus(GL_FRAMEBUFFER));

        // Unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    int OpenGLFramebuffer::readPixel(unsigned attachmentIndex, unsigned x, unsigned y)
    {
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        int pixel;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
        return pixel;
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

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, image->getId(), 0);
        if(_stencilAttachmentIndex == _depthAttachmentIndex)
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, image->getId(), 0);

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
            case Image::Format::DEPTH24_STENCIL8: return GL_DEPTH_ATTACHMENT;
            default: break;
        }
        ASSERT(false, "Could not convert format to openGL depth attachment");
    }
}
