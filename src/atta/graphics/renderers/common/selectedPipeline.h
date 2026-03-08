// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
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
