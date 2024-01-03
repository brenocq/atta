//--------------------------------------------------
// Atta Graphics Module
// gridPipeline.h
// Date: 2021-11-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERERS_COMMON_GRID_PIPELINE_H
#define ATTA_GRAPHICS_RENDERERS_COMMON_GRID_PIPELINE_H
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/pipeline.h>
#include <atta/graphics/renderQueue.h>

namespace atta::graphics {

class GridPipeline final {
  public:
    GridPipeline(std::shared_ptr<RenderPass> renderPass);
    ~GridPipeline();

    /// Update GPU line grid
    void update(std::shared_ptr<Camera> camera);

    /**
     * @brief Render lines from grid
     *
     * @note GridPipeline::update should be called before rendering to make sure that the GPU data is up to date
     */
    void render(std::shared_ptr<Camera> camera);

  private:
    std::shared_ptr<Pipeline> _pipeline;

    struct Line {
        vec3 pos;
        vec4 color;
        float width;
    };
    static constexpr size_t MAX_LINES = 1000;
    std::array<Line, MAX_LINES> _lines;
    size_t _numLines;

    StringId _gridMeshName;
    static size_t _gridId;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERERS_COMMON_GRID_PIPELINE_H
