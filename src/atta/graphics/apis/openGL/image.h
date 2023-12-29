//--------------------------------------------------
// Atta Graphics Module
// image.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_IMAGE_H
#define ATTA_GRAPHICS_APIS_OPENGL_IMAGE_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/image.h>

namespace atta::graphics::gl {

class Image final : public gfx::Image {
  public:
    Image(const Image::CreateInfo& info);
    ~Image();

    static GLenum convertSizedInternalFormat(Format format);
    static GLenum convertFormat(Format format);
    static GLenum convertInternalFormat(Format format);
    static GLenum convertDataType(Format format);
    static GLenum convertSamplerWrap(Wrap samplerWrap);

    void write(uint8_t* data) override;
    std::vector<uint8_t> read(vec2i offset = {0, 0}, vec2i size = {0, 0}) override;
    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    OpenGLId getHandle() const { return _id; }

    void* getImGuiImage() override { return reinterpret_cast<void*>(OpenGLId(_id)); }

    using FramebufferRead = std::function<std::vector<uint8_t>(vec2i, vec2i)>;
    void setFramebufferRead(std::function<std::vector<uint8_t>(vec2i, vec2i)> framebufferRead);

  private:
    OpenGLId _id;

    /// In OpenGL we need the framebuffer to read the attachment image
    FramebufferRead _framebufferRead;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_IMAGE_H
