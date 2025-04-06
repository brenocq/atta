//--------------------------------------------------
// Atta Graphics Module
// skyboxPipeline.h
// Date: 2025-04-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_COMMON_SKYBOX_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_COMMON_SKYBOX_PIPELINE_H
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics {

class SkyboxPipeline final {
  public:
    SkyboxPipeline(std::shared_ptr<RenderPass> renderPass);

    /// Render skybox
    void render(std::shared_ptr<Camera> camera, StringId envImg);

  private:
    std::shared_ptr<Pipeline> _pipeline;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_COMMON_SKYBOX_PIPELINE_H
