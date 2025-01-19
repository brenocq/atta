//--------------------------------------------------
// Atta Graphics Module
// entityClick.h
// Date: 2021-11-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_COMPUTE_ENTITY_CLICK_H
#define ATTA_GRAPHICS_COMPUTE_ENTITY_CLICK_H

#include <atta/component/base.h>
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderPass.h>
#include <atta/graphics/renderers/renderer.h>
#include <atta/utils/math/math.h>

namespace atta::graphics {

class EntityClick {
  public:
    EntityClick();

    component::EntityId click(std::shared_ptr<Renderer> renderer, std::shared_ptr<Camera> camera, vec2i pos);

  private:
    uint32_t _width;
    uint32_t _height;

    std::shared_ptr<RenderQueue> _renderQueue;
    std::shared_ptr<RenderPass> _renderPass;
    std::shared_ptr<Pipeline> _geometryPipeline;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_COMPUTE_ENTITY_CLICK_H
