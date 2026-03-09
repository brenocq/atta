// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics {

class Irradiance {
  public:
    Irradiance();

    /**
     * @brief Create an irradiance cubemap from an environment cubemap
     *
     * The resulting cubemap encodes the pre-integrated diffuse irradiance for every
     * possible surface normal direction, enabling real-time IBL diffuse lighting.
     *
     * @param environmentCubemapSid Name of the environment cubemap gfx::Image to convolve
     * @return The irradiance cubemap image, already transitioned to shader-read layout
     */
    std::shared_ptr<gfx::Image> createIrradianceCubemap(StringId environmentCubemapSid);

  private:
    uint32_t _width;
    uint32_t _height;

    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<Pipeline> _pipeline;
};

} // namespace atta::graphics
