//--------------------------------------------------
// Atta Graphics Module
// selectedPipeline.h
// Date: 2021-11-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_COMMON_SELECTED_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_COMMON_SELECTED_PIPELINE_H
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics {
class SelectedPipeline final {
  public:
    SelectedPipeline(std::shared_ptr<RenderPass> renderPass, VertexBufferLayout layout);

    void render(std::shared_ptr<Camera> camera);

  private:
    std::shared_ptr<Pipeline> _pipeline;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_COMMON_SELECTED_PIPELINE_H
