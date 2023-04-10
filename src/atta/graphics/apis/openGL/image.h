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

    void write(void* data) override;
    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    void* getImGuiImage() override { return reinterpret_cast<void*>(OpenGLId(_id)); }
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_IMAGE_H
