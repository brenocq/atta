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
    _depthAttachmentIndex = -1;
    _stencilAttachmentIndex = -1;
    for (auto attachment : _attachments) {
        // Check attachment consistency
        DASSERT(!(attachment.image != nullptr && attachment.format != Image::Format::NONE),
                "Should not create an attachment with format and image defined. Plese check the attachments for [w]$0", _debugName);

        // Check only one depth and stencil attachments
        Image::Format format = attachment.image != nullptr ? attachment.image->getFormat() : attachment.format;
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

std::shared_ptr<Image> Framebuffer::getImage(uint32_t attachment) {
    DASSERT(_images.find(attachment) != _images.end(), "[$0] Trying to access invalid attachment $1", _debugName.getString(), attachment);
    return _images[attachment];
}

} // namespace atta::graphics
