//--------------------------------------------------
// Atta Graphics System
// rendererAPI.h
// Date: 2021-08-24
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#define ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
#include <atta/core/stringId.h>
#include <atta/graphicsSystem/framebuffer.h>
#include <atta/graphicsSystem/image.h>

namespace atta
{
    class ShaderGroup;
    class RendererAPI
    {
    public:
        enum Type
        {
            OPENGL = 0,
            VULKAN
        };

        RendererAPI(Type type);
        virtual ~RendererAPI() = default;

        virtual void beginFrame() = 0;
        virtual void endFrame() = 0;

        virtual void renderMesh(StringId meshSid) = 0;
        virtual void renderQuad() = 0;
        virtual void renderQuad3() = 0;
        virtual void renderCube() = 0;
        virtual void renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) = 0;

        virtual void generateCubemap(StringId textureSid, mat4 rotationMatrix = mat4(1.0f)) = 0;
        struct GenerateProcessedCubemapInfo
        {
            StringId cubemapSid = StringId("Not defined");
            std::shared_ptr<ShaderGroup> shader;
            unsigned width = 128;// New cubemap face width
            unsigned height = 128;// New cubemap face height 
            int numMipLevels = 1;
            std::function<void(std::shared_ptr<ShaderGroup> shader, mat4 proj, mat4 view, int face, int mipLevel)> func;// Function called for each mip level, each face
        };
        virtual void generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) = 0;

        struct GenerateProcessedTextureInfo
        {
            StringId textureSid = StringId("Not defined");
            std::shared_ptr<ShaderGroup> shader;
            Image::CreateInfo imageInfo;
            std::function<void(std::shared_ptr<ShaderGroup> shader)> func;
        };
        virtual void generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) = 0;

        virtual void* getImGuiImage(StringId sid) const = 0;

        Type getType() const { return _type; }

    protected:
        Type _type;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERER_API_H
