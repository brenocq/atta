//--------------------------------------------------
// Atta Graphics Module
// pbrRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_PBR_RENDERER_H
#define ATTA_GRAPHICS_RENDERERS_PBR_RENDERER_H

#include <atta/graphics/image.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderers/common/drawerPipeline.h>
#include <atta/graphics/renderers/common/selectedPipeline.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::graphics {

class PbrRenderer final : public Renderer {
  public:
    PbrRenderer();
    ~PbrRenderer();

    void render(std::shared_ptr<Camera> camera) override;
    void resize(uint32_t width, uint32_t height) override;

    void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }
    std::shared_ptr<Framebuffer> getFramebuffer() override { return _geometryPipeline->getRenderPass()->getFramebuffer(); }

  private:
    void shadowPass();
    void geometryPass(std::shared_ptr<Camera> camera);

    void irradianceCubemap();
    void prefilterCubemap();
    void brdfLUT();

    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _geometryRenderPass;
    std::shared_ptr<Pipeline> _geometryPipeline;
    std::unique_ptr<SelectedPipeline> _selectedPipeline;
    std::unique_ptr<DrawerPipeline> _drawerPipeline;

    std::shared_ptr<Shader> _backgroundShader;
    bool _firstRender;
    bool _wasResized;

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

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_PBR_RENDERER_H
