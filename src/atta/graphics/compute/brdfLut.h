// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics {

class BrdfLut {
  public:
    BrdfLut();

    /**
     * @brief Generate the BRDF lookup table
     *
     * Pre-integrates the Cook-Torrance BRDF for all (NdotV, roughness) pairs.
     * The result is stored in a 2D texture: R = scale (A), G = bias (B).
     * Used in the split-sum specular IBL approximation.
     *
     * @return The BRDF LUT image, already transitioned to shader-read layout
     */
    std::shared_ptr<gfx::Image> generate();

  private:
    uint32_t _width;
    uint32_t _height;

    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<Pipeline> _pipeline;
};

} // namespace atta::graphics
