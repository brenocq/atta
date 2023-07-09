//--------------------------------------------------
// Atta Graphics Module
// graphicsAPI.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_GRAPHICS_API_H
#define ATTA_GRAPHICS_APIS_GRAPHICS_API_H

#include <atta/graphics/framebuffer.h>
#include <atta/graphics/image.h>
#include <atta/graphics/windows/window.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Shader;
class GraphicsAPI {
  public:
    enum Type { OPENGL = 0, VULKAN };

    GraphicsAPI(Type type, std::shared_ptr<Window> window);
    virtual ~GraphicsAPI() = default;

    virtual void startUp() = 0;
    virtual void shutDown() = 0;
    virtual void waitDevice() = 0;

    virtual void beginFrame() = 0;
    virtual void endFrame() = 0;

    virtual void renderMesh(StringId meshSid) = 0;
    virtual void renderQuad() = 0;
    virtual void renderQuad3() = 0;
    virtual void renderCube() = 0;
    virtual void renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) = 0;

    virtual void generateCubemap(StringId textureSid, mat4 rotationMatrix = mat4(1.0f)) = 0;

    struct GenerateProcessedCubemapInfo {
        StringId cubemapSid = StringId("Not defined");
        std::shared_ptr<Shader> shader;
        unsigned width = 128;  // New cubemap face width
        unsigned height = 128; // New cubemap face height
        int numMipLevels = 1;
        std::function<void(std::shared_ptr<Shader> shader, mat4 proj, mat4 view, int face, int mipLevel)>
            func; // Function called for each mip level, each face
    };
    virtual void generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) = 0;

    struct GenerateProcessedTextureInfo {
        StringId textureSid = StringId("Not defined");
        std::shared_ptr<Shader> shader;
        Image::CreateInfo imageInfo;
        std::function<void(std::shared_ptr<Shader> shader)> func;
    };
    virtual void generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) = 0;

    virtual void* getImGuiImage(StringId sid) const = 0;

    Type getType() const { return _type; }

  protected:
    Type _type;
    std::shared_ptr<Window> _window;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_APIS_GRAPHICS_API_H
