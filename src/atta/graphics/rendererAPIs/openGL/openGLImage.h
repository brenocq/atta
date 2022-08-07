//--------------------------------------------------
// Atta Graphics Module
// openGLImage.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_IMAGE_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_IMAGE_H
#include <atta/graphics/image.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>

namespace atta::graphics {
class OpenGLImage final : public Image {
  public:
    OpenGLImage(const Image::CreateInfo& info);
    ~OpenGLImage();

    static GLenum convertSizedInternalFormat(Format format);
    static GLenum convertFormat(Format format);
    static GLenum convertInternalFormat(Format format);
    static GLenum convertDataType(Format format);
    static GLenum convertSamplerWrap(Wrap samplerWrap);

    void write(void* data) override;
    void resize(uint32_t width, uint32_t height, bool forceRecreate = false) override;

    void* getImGuiImage() override { return reinterpret_cast<void*>(OpenGLId(_id)); }
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_IMAGE_H
