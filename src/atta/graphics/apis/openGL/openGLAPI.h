//--------------------------------------------------
// Atta Graphics Module
// openGLAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_OPENGL_RENDERER_H
#define ATTA_GRAPHICS_APIS_OPENGL_OPENGL_RENDERER_H

#include <atta/graphics/apis/graphicsAPI.h>
#include <atta/graphics/apis/openGL/image.h>
#include <atta/graphics/apis/openGL/mesh.h>
#include <atta/graphics/apis/openGL/shader.h>
#include <atta/graphics/windows/window.h>

namespace atta::graphics {

class OpenGLAPI final : public GraphicsAPI {
  public:
    OpenGLAPI(std::shared_ptr<Window> window);
    ~OpenGLAPI();

    void startUp() override;
    void shutDown() override;
    void waitDevice() override;

    void beginFrame() override;
    void endFrame() override;

    void renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) override;

    void generateCubemap(StringId textureSid, mat4 rotationMatrix = mat4(1.0f)) override;
    void generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) override;
    void generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) override;

    void* getImGuiImage(StringId sid) const override;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_APIS_OPENGL_OPENGL_RENDERER_H
