//--------------------------------------------------
// Atta Graphics Module
// framebuffer.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_FRAMEBUFFER_H
#define ATTA_GRAPHICS_APIS_OPENGL_FRAMEBUFFER_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/framebuffer.h>

namespace atta::graphics::gl {

class Framebuffer final : public gfx::Framebuffer {
  public:
    Framebuffer(const Framebuffer::CreateInfo& info);
    ~Framebuffer();

    void bind(bool clear = true) override;
    void unbind() override;

    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    static GLenum convertDepthAttachmentType(Image::Format format);

  private:
    void createAttachments();
    void bindAttachments();

    OpenGLId _id;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_FRAMEBUFFER_H
