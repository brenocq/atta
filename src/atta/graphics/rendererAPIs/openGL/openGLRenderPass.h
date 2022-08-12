//--------------------------------------------------
// Atta Graphics Module
// openGLRenderPass.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_RENDER_PASS_H
#define ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_RENDER_PASS_H
#include <atta/graphics/renderPass.h>

namespace atta::graphics {

class OpenGLRenderPass final : public RenderPass {
  public:
    OpenGLRenderPass(const RenderPass::CreateInfo& info);
    ~OpenGLRenderPass();

    void begin(bool clear = true) override;
    void end() override;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_APIS_OPENGL_OPENGL_RENDER_PASS_H
