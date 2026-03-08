// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#ifndef ATTA_GRAPHICS_RENDERERS_COMMON_SELECTED_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_COMMON_SELECTED_PIPELINE_H
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics {

class SelectedPipeline final {
  public:
    SelectedPipeline(std::shared_ptr<RenderPass> renderPass);

    void render(std::shared_ptr<Camera> camera);

  private:
    std::shared_ptr<Pipeline> _pipeline;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_COMMON_SELECTED_PIPELINE_H
