//--------------------------------------------------
// Atta Graphics Module
// equiToCubemap.h
// Date: 2025-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_COMPUTE_EQUI_TO_CUBEMAP_H
#define ATTA_GRAPHICS_COMPUTE_EQUI_TO_CUBEMAP_H

#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics {

class EquiToCubemap {
  public:
    EquiToCubemap();

    /**
     * @brief Create a cubemap image from an equirectangular image
     *
     * @param textureSid The name image resource to be created from
     * @param rotationMatrix The rotation matrix to be applied to the cubemap
     */
    std::shared_ptr<gfx::Image> createCubemap(StringId imageSid);

  private:
    // Cubemap face width/height
    uint32_t _width;
    uint32_t _height;

    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<Pipeline> _pipeline;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_COMPUTE_EQUI_TO_CUBEMAP_H
