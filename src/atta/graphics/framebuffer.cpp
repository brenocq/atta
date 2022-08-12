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
}

Framebuffer::~Framebuffer() { _images.clear(); }

std::shared_ptr<Image> Framebuffer::getImage(uint32_t attachment) {
    DASSERT(_images.find(attachment) != _images.end(), "[$0] Trying to access invalid attachment $1", _debugName.getString(), attachment);
    return _images[attachment];
}

} // namespace atta::graphics
