//--------------------------------------------------
// Atta Graphics Module
// framebuffer.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/framebuffer.h>

namespace atta::graphics {

Framebuffer::Framebuffer(const CreateInfo& info)
    : _width(info.width), _height(info.height), _clearColor(info.clearColor), _attachments(info.attachments), _debugName(info.debugName) {
    DASSERT(_attachments.size() > 0, "It is not possible to create framebuffer with no attachments");

    // Check consistency and populate color and depth attachments
    int i = 0;
    _colorAttachmentIndex = -1;
    _depthAttachmentIndex = -1;
    _stencilAttachmentIndex = -1;
    for (auto attachment : _attachments) {
        // Check attachment consistency
        DASSERT(!(attachment.image != nullptr && attachment.format != Image::Format::NONE),
                "Should not create an attachment with format and image defined. Plese check the attachments for [w]$0", _debugName);

        // Check only one color/depth/stencil attachments
        Image::Format format = attachment.image != nullptr ? attachment.image->getFormat() : attachment.format;

        if (Image::isColorFormat(format)) {
            DASSERT(_colorAttachmentIndex == -1, "It is not possible to create framebuffer with more than one color attachment");
            _colorAttachmentIndex = i;
        }

        if (Image::isDepthFormat(format)) {
            DASSERT(_depthAttachmentIndex == -1, "It is not possible to create framebuffer with more than one depth attachment");
            _depthAttachmentIndex = i;
        }

        if (Image::isStencilFormat(format)) {
            DASSERT(_stencilAttachmentIndex == -1, "It is not possible to create framebuffer with more than one stencil attachment");
            _stencilAttachmentIndex = i;
        }
        i++;
    }
}

Framebuffer::~Framebuffer() { _images.clear(); }

std::vector<std::shared_ptr<Image>> Framebuffer::getImages() const { return _images; }

std::shared_ptr<Image> Framebuffer::getImage(uint32_t attachment) {
    DASSERT(attachment < _images.size(), "[$0] Trying to access invalid attachment $1", _debugName.getString(), attachment);
    return _images[attachment];
}

bool Framebuffer::hasColorAttachment() const { return _colorAttachmentIndex >= 0; }
bool Framebuffer::hasDepthAttachment() const { return _depthAttachmentIndex >= 0; }
bool Framebuffer::hasStencilAttachment() const { return _stencilAttachmentIndex >= 0; }
int Framebuffer::getColorAttachmentIndex() const { return _colorAttachmentIndex; }
int Framebuffer::getDepthAttachmentIndex() const { return _depthAttachmentIndex; }
int Framebuffer::getStencilAttachmentIndex() const { return _stencilAttachmentIndex; }

} // namespace atta::graphics
