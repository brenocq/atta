//--------------------------------------------------
// Atta Graphics Module
// vulkanRenderer.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_VULKAN_VULKAN_RENDERER_H
#define ATTA_GRAPHICS_RENDERER_APIS_VULKAN_VULKAN_RENDERER_H
#include <atta/graphics/rendererAPI.h>

namespace atta
{
    class VulkanRenderer final : public RendererAPI
    {
    public:
        VulkanRenderer();
        ~VulkanRenderer();

        void beginFrame() override;
        void endFrame() override;

        void beginRenderPass() override;
        void endRenderPass() override;

        void renderMesh() override;
        void renderQuad() override;
        void renderQuad3() override;
        void renderCube() override;
        void renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) override;

        void generateCubemap(StringId textureSid, mat4 rotationMatrix = mat4(1.0f)) override;
        void generatePorcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) override;
        void generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) override;
    };
}

#endif// ATTA_GRAPHICS_RENDERER_APIS_VULKAN_VULKAN_RENDERER_H
