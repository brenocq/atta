//--------------------------------------------------
// Atta Graphics Module
// openGLFramebuffer.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
#include <atta/graphics/framebuffer.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>

namespace atta::graphics {
class OpenGLFramebuffer final : public Framebuffer {
  public:
    OpenGLFramebuffer(const Framebuffer::CreateInfo& info);
    ~OpenGLFramebuffer();

    void bind(bool clear = true) override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    int readPixel(unsigned attachmentIndex, unsigned x, unsigned y) override;
    std::vector<uint8_t> readImage(unsigned attachmentIndex) override;

    static GLenum convertDepthAttachmentType(Image::Format format);

  private:
    void createAttachments();
    void bindAttachments();

    OpenGLId _id;

    // Attachments
    int _depthAttachmentIndex;
    int _stencilAttachmentIndex;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_FRAMEBUFFER_H
