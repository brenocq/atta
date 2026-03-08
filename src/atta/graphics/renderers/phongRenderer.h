// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderers/common/drawerPipeline.h>
#include <atta/graphics/renderers/common/gridPipeline.h>
#include <atta/graphics/renderers/common/selectedPipeline.h>
#include <atta/graphics/renderers/renderer.h>

namespace atta::graphics {

class PhongRenderer final : public Renderer {
  public:
    PhongRenderer();
    ~PhongRenderer();

    void render(std::shared_ptr<Camera> camera) override;
    void resize(uint32_t width, uint32_t height) override;

    void* getImGuiTexture() const override { return _geometryPipeline->getImGuiTexture(); }
    std::shared_ptr<Framebuffer> getFramebuffer() override { return _geometryPipeline->getRenderPass()->getFramebuffer(); }

  private:
    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<Pipeline> _geometryPipeline;
    std::unique_ptr<DrawerPipeline> _drawerPipeline;
    std::unique_ptr<GridPipeline> _gridPipeline;
    std::unique_ptr<SelectedPipeline> _selectedPipeline;

    bool _wasResized;
};

} // namespace atta::graphics
