//--------------------------------------------------
// Atta Graphics Module
// phongRenderer.h
// Date: 2021-09-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_PHONG_RENDERER_H
#define ATTA_GRAPHICS_RENDERERS_PHONG_RENDERER_H
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderers/renderer.h>
#include <atta/graphics/renderers/common/selectedPipeline.h>
#include <atta/graphics/renderers/common/drawerPipeline.h>

namespace atta
{
    class PhongRenderer final : public Renderer
    {
    public:
        PhongRenderer();
        ~PhongRenderer();

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
}

#endif// ATTA_GRAPHICS_RENDERERS_PHONG_RENDERER_H
