//--------------------------------------------------
// Atta Graphics Module
// pipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_PIPELINE_H
#define ATTA_GRAPHICS_APIS_OPENGL_PIPELINE_H

#include <atta/graphics/apis/openGL/base.h>
#include <atta/graphics/pipeline.h>

namespace atta::graphics::gl {

class Pipeline final : public gfx::Pipeline {
  public:
    Pipeline(const Pipeline::CreateInfo& info);
    ~Pipeline();

    void begin(std::shared_ptr<RenderQueue> renderQueue) override;
    void end() override;

    void* getImGuiTexture() const override;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_PIPELINE_H
