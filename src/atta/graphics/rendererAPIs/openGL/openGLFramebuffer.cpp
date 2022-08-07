//--------------------------------------------------
// Atta Graphics Module
// openGLFramebuffer.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/rendererAPIs/openGL/openGLFramebuffer.h>
#include <atta/graphics/rendererAPIs/openGL/openGLImage.h>

namespace atta
{
    OpenGLFramebuffer::OpenGLFramebuffer(const Framebuffer::CreateInfo& info):
        Framebuffer(info), _id(0), _depthAttachmentIndex(-1), _stencilAttachmentIndex(-1)
    {
        // Check consistency and populate color and depth attachments
        int i = 0;
        for(auto attachment : _attachments)
        {
            // Check attachment consistency
            DASSERT(!(attachment.image != nullptr && attachment.format != Image::Format::NONE), "Should not create an attachment with format and image defined. Plese check the attachments for [w]$0", _debugName);

            // Check only one depth and stencil attachments
            Image::Format format = attachment.image != nullptr ? attachment.image->getFormat() : attachment.format;
            if(Image::isDepthFormat(format))
            {
                DASSERT(_depthAttachmentIndex == -1, "It is not possible to create framebuffer with more than one depth attachment");
                _depthAttachmentIndex = i;
            }

            if(Image::isStencilFormat(format))
            {
                DASSERT(_stencilAttachmentIndex == -1, "It is not possible to create framebuffer with more than one stencil attachment");
                _stencilAttachmentIndex = i;
            }
            i++;
        }

        // Create attachments
        createAttachments();

        resize(_width, _height, true);
    }

    OpenGLFramebuffer::~OpenGLFramebuffer()
    {
        if(_id)
            glDeleteFramebuffers(1, &_id);
    }

    void OpenGLFramebuffer::createAttachments()
    {
        // Create attachment images
        for(unsigned i = 0; i < _attachments.size(); i++)
        {
            std::shared_ptr<OpenGLImage> image;
            if(_attachments[i].image)
            {
                // If already created
                image = std::dynamic_pointer_cast<OpenGLImage>(_attachments[i].image);
            }
            else
            {
                // If create from format
                Image::CreateInfo info;
                info.format = _attachments[i].format;
                info.width = _width;
                info.height = _height;
                image = std::make_shared<OpenGLImage>(info);
            }
            _images[i] = image;
        }
    }

    void OpenGLFramebuffer::bindAttachments()
    {
        // Bind attachment images
        unsigned colorIndex = 0;

        for(unsigned i = 0; i < _images.size(); i++)
        {
            bool isColor = (i != _depthAttachmentIndex) && (i != _stencilAttachmentIndex);
            if(i == _depthAttachmentIndex)
            {
                if(!_images[i]->isCubemap())
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _images[i]->getId(), 0);
                else
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _images[i]->getId(), 0);
            }
            if(i == _stencilAttachmentIndex)
            {
                if(!_images[i]->isCubemap())
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _images[i]->getId(), 0);
                else
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, _images[i]->getId(), 0);
            }

            if(isColor)
            {
                if(!_images[i]->isCubemap())
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+colorIndex, GL_TEXTURE_2D, _images[i]->getId(), 0);
                else
                    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+colorIndex, _images[i]->getId(), 0);
                colorIndex++;
            }
        }
    }

    void OpenGLFramebuffer::bind(bool clear)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glViewport(0, 0, _width, _height);
        if(clear)
        {
            // TODO Renderer choose clear color
            if(_attachments[0].format == Image::Format::RGB)
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
            glDeleteFramebuffers(1, &_id);

        glGenFramebuffers(1, &_id);
        glBindFramebuffer(GL_FRAMEBUFFER, _id);

        //----- Update attachment images -----//
        // Resize attachments
        for(unsigned i = 0; i < _attachments.size(); i++)
            _images[i]->resize(width, height, forceRecreate);

        // Bind attachments
        bindAttachments();

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

    std::vector<uint8_t> OpenGLFramebuffer::readImage(unsigned attachmentIndex)
    {
        int numChannels = Image::getFormatSize(_attachments[attachmentIndex].format);
        GLenum formatOpenGL = OpenGLImage::convertFormat(_attachments[attachmentIndex].format);
        GLenum dataTypeOpenGL = OpenGLImage::convertDataType(_attachments[attachmentIndex].format);

        glBindFramebuffer(GL_FRAMEBUFFER, _id);
        glViewport(0, 0, _width, _height);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        std::vector<uint8_t> data(_width*_height*numChannels);
        glReadPixels(0, 0, _width, _height, formatOpenGL, dataTypeOpenGL, &data[0]);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return data;
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
