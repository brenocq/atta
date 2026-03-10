// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once

#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics {

class Prefilter {
  public:
    Prefilter();

    /**
     * @brief Create a prefiltered specular cubemap from an environment cubemap
     *
     * Generates a cubemap with multiple mip levels, each representing the environment
     * convolved with a GGX lobe of increasing roughness (mip 0 = mirror-like, mip 4 = fully rough).
     * Used for the specular IBL split-sum approximation.
     *
     * @param environmentCubemapSid Name of the environment cubemap gfx::Image to convolve
     * @return The prefiltered cubemap image with mip levels, already transitioned to shader-read layout
     */
    std::shared_ptr<gfx::Image> createPrefilterCubemap(StringId environmentCubemapSid);

  private:
    static constexpr uint32_t _baseSize = 128;
    static constexpr uint32_t _numMips = 5;
};

} // namespace atta::graphics
