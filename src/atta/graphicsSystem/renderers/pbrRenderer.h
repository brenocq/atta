//--------------------------------------------------
// Atta Graphics System
// pbrRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERERS_PBR_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERERS_PBR_RENDERER_H
#include <atta/graphicsSystem/pipeline.h>
#include <atta/graphicsSystem/image.h>
#include <atta/graphicsSystem/renderers/renderer.h>
#include <atta/graphicsSystem/renderers/common/selectedPipeline.h>
#include <atta/graphicsSystem/renderers/common/drawerPipeline.h>

namespace atta
{
    class PbrRenderer final : public Renderer
    {
    public:
        PbrRenderer();
        ~PbrRenderer();

        void render(std::shared_ptr<Camera> camera) override;
        void resize(uint32_t width, uint32_t height) override;

        uint32_t getWidth() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getWidth(); };
        uint32_t getHeight() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getHeight(); };
        void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }
        std::shared_ptr<Framebuffer> getFramebuffer() { return _geometryPipeline->getRenderPass()->getFramebuffer(); }

    private:
        void shadowPass();
        void geometryPass(std::shared_ptr<Camera> camera);

        void irradianceCubemap();
        void prefilterCubemap();
        void brdfLUT();

        std::shared_ptr<Pipeline> _geometryPipeline;
        std::unique_ptr<SelectedPipeline> _selectedPipeline;
        std::unique_ptr<DrawerPipeline> _drawerPipeline;

        std::shared_ptr<ShaderGroup> _backgroundShader;
        bool _firstRender;

        //----- Lighting -----//
        // Directional light
        mat3 _directionalLightMatrix;
        // Environment light
        StringId _lastEnvironmentMap;
        mat3 _environmentMapOri;

        // Shadow mapping
        std::shared_ptr<Pipeline> _shadowMapPipeline;
        std::shared_ptr<Pipeline> _omniShadowMapPipeline;
        std::shared_ptr<Image> _omnidirectionalShadowMap;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERERS_PBR_RENDERER_H
