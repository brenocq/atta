//--------------------------------------------------
// Atta Graphics Module
// drawerPipeline.h
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_COMMON_DRAWER_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_COMMON_DRAWER_PIPELINE_H
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics {

class DrawerPipeline final {
  public:
    DrawerPipeline(std::shared_ptr<RenderPass> renderPass);

    /**
     * @brief Update GPU data
     *
     * @note It should be called before render to make sure the GPU data is up to date
     */
    void update();

    void render(std::shared_ptr<Camera> camera);

  private:
    std::shared_ptr<Pipeline> _linePipeline;
    std::shared_ptr<Pipeline> _pointPipeline;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_COMMON_DRAWER_PIPELINE_H
