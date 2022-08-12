//--------------------------------------------------
// Atta Graphics Module
// fastRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_FAST_RENDERER_H
#define ATTA_GRAPHICS_RENDERERS_FAST_RENDERER_H

#include <atta/graphics/pipeline.h>
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

    uint32_t getWidth() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getWidth(); };
    uint32_t getHeight() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getHeight(); };
    void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }
    std::shared_ptr<Framebuffer> getFramebuffer() { return _geometryPipeline->getRenderPass()->getFramebuffer(); }

  private:
    std::shared_ptr<Pipeline> _geometryPipeline;
    std::unique_ptr<SelectedPipeline> _selectedPipeline;
    std::unique_ptr<DrawerPipeline> _drawerPipeline;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_FAST_RENDERER_H
