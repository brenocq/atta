//--------------------------------------------------
// Atta Graphics Module
// openGLPipeline.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_PIPELINE_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_PIPELINE_H
#include <atta/graphics/pipeline.h>
#include <atta/graphics/rendererAPIs/openGL/base.h>

namespace atta::graphics {
class OpenGLPipeline final : public Pipeline {
  public:
    OpenGLPipeline(const Pipeline::CreateInfo& info);
    ~OpenGLPipeline();

    void begin(bool clear = true) override;
    void end() override;

    void* getImGuiTexture() const override;
};
} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_PIPELINE_H
