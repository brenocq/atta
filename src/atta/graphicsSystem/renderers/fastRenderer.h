//--------------------------------------------------
// Atta Graphics System
// fastRenderer.h
// Date: 2021-09-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_RENDERERS_FAST_RENDERER_H
#define ATTA_GRAPHICS_SYSTEM_RENDERERS_FAST_RENDERER_H
#include <atta/graphicsSystem/pipeline.h>
#include <atta/graphicsSystem/renderers/renderer.h>
#include <atta/graphicsSystem/renderers/common/selectedPipeline.h>
#include <atta/graphicsSystem/renderers/common/drawerPipeline.h>

namespace atta
{
    class FastRenderer final : public Renderer
    {
    public:
        FastRenderer();
        ~FastRenderer();

        void render(std::shared_ptr<Camera> camera) override;
        void resize(uint32_t width, uint32_t height) override;

        uint32_t getWidth() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getWidth(); };
        uint32_t getHeight() const override { return _geometryPipeline->getRenderPass()->getFramebuffer()->getHeight(); };
        void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }

    private:
        std::shared_ptr<Pipeline> _geometryPipeline;
        std::unique_ptr<SelectedPipeline> _selectedPipeline;
        std::unique_ptr<DrawerPipeline> _drawerPipeline;
    };
}

#endif// ATTA_GRAPHICS_SYSTEM_RENDERERS_FAST_RENDERER_H
