//--------------------------------------------------
// Atta Graphics Module
// fastRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_FAST_RENDERER_H
#define ATTA_GRAPHICS_RENDERERS_FAST_RENDERER_H

#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderers/common/drawerPipeline.h>
#include <atta/graphics/renderers/common/selectedPipeline.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::graphics {

class FastRenderer final : public Renderer {
  public:
    FastRenderer();
    ~FastRenderer();

    void render(std::shared_ptr<Camera> camera) override;
    void resize(uint32_t width, uint32_t height) override;

    void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }
    std::shared_ptr<Framebuffer> getFramebuffer() override { return _geometryPipeline->getRenderPass()->getFramebuffer(); }

  private:
    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<Pipeline> _geometryPipeline;
    std::unique_ptr<SelectedPipeline> _selectedPipeline;
    std::unique_ptr<DrawerPipeline> _drawerPipeline;
    bool _wasResized;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_FAST_RENDERER_H
