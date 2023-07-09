//--------------------------------------------------
// Atta Graphics Module
// framebuffer.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/framebuffer.h>
#include <atta/graphics/apis/openGL/image.h>

namespace atta::graphics::gl {

Framebuffer::Framebuffer(const Framebuffer::CreateInfo& info) : gfx::Framebuffer(info), _id(0) {
    // Create attachments
    createAttachments();
    resize(_width, _height, true);
}

Framebuffer::~Framebuffer() {
    if (_id)
        glDeleteFramebuffers(1, &_id);
}

void Framebuffer::createAttachments() {
    // Create attachment images
    for (unsigned i = 0; i < _attachments.size(); i++) {
        std::shared_ptr<Image> image;
        if (_attachments[i].image) {
            // If already created
            image = std::dynamic_pointer_cast<Image>(_attachments[i].image);
        } else {
            // If create from format
            Image::CreateInfo info;
            info.format = _attachments[i].format;
            info.width = _width;
            info.height = _height;
            image = std::make_shared<Image>(info);
        }
        _images.push_back(image);
    }
}

void Framebuffer::bindAttachments() {
    // Bind attachment images
    unsigned colorIndex = 0;

    for (unsigned i = 0; i < _images.size(); i++) {
        std::shared_ptr<Image> image = std::dynamic_pointer_cast<Image>(_images[i]);
        bool isColor = (i != _depthAttachmentIndex) && (i != _stencilAttachmentIndex);
        if (i == _depthAttachmentIndex) {
            if (!image->isCubemap())
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, image->getHandle(), 0);
            else
                glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, image->getHandle(), 0);
        }
        if (i == _stencilAttachmentIndex) {
            if (!image->isCubemap())
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, image->getHandle(), 0);
            else
                glFramebufferTexture(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, image->getHandle(), 0);
        }

        if (isColor) {
            if (!image->isCubemap())
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, GL_TEXTURE_2D, image->getHandle(), 0);
            else
                glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorIndex, image->getHandle(), 0);
            colorIndex++;
        }
    }
}

void Framebuffer::bind(bool clear) {
    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glViewport(0, 0, _width, _height);
    if (clear) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // TODO Renderer should choose clear color
        if (_attachments[0].format == Image::Format::RGB || _attachments[0].format == Image::Format::RGBA)
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        if (_depthAttachmentIndex != -1 && _stencilAttachmentIndex != -1) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_STENCIL_TEST);
            glStencilMask(0x00);
        } else if (_depthAttachmentIndex != -1) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
        } else
            glClear(GL_COLOR_BUFFER_BIT);
    }
}

void Framebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void Framebuffer::resize(uint32_t width, uint32_t height, bool forceRecreate) {
    if (!forceRecreate && width == _width && height == _height)
        return;

    _width = width;
    _height = height;

    if (_id)
        glDeleteFramebuffers(1, &_id);

    glGenFramebuffers(1, &_id);
    glBindFramebuffer(GL_FRAMEBUFFER, _id);

    //----- Update attachment images -----//
    // Resize attachments
    for (unsigned i = 0; i < _attachments.size(); i++)
        _images[i]->resize(width, height, forceRecreate);

    // Bind attachments
    bindAttachments();

    //----- Check framebuffer-----//
    ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete and can not be created ($0)",
           glCheckFramebufferStatus(GL_FRAMEBUFFER));

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Framebuffer::readPixel(unsigned attachmentIndex, unsigned x, unsigned y) {
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixel;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
    return pixel;
}

std::vector<uint8_t> Framebuffer::readImage(unsigned attachmentIndex) {
    int numChannels = Image::getNumChannels(_attachments[attachmentIndex].format);
    GLenum formatOpenGL = Image::convertFormat(_attachments[attachmentIndex].format);
    GLenum dataTypeOpenGL = Image::convertDataType(_attachments[attachmentIndex].format);

    glBindFramebuffer(GL_FRAMEBUFFER, _id);
    glViewport(0, 0, _width, _height);
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    std::vector<uint8_t> data(_width * _height * numChannels);
    glReadPixels(0, 0, _width, _height, formatOpenGL, dataTypeOpenGL, &data[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return data;
}

//------------------------------------------------//
//---------- Atta to OpenGL conversions ----------//
//------------------------------------------------//
GLenum Framebuffer::convertDepthAttachmentType(Image::Format format) {
    switch (format) {
        case Image::Format::NONE:
            break;
        case Image::Format::DEPTH32F:
            return GL_DEPTH_ATTACHMENT;
        case Image::Format::DEPTH24_STENCIL8:
            return GL_DEPTH_ATTACHMENT;
        default:
            break;
    }
    ASSERT(false, "Could not convert format to openGL depth attachment");
}

} // namespace atta::graphics::gl