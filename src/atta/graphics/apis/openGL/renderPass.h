//--------------------------------------------------
// Atta Graphics Module
// renderPass.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_RENDER_PASS_H
#define ATTA_GRAPHICS_APIS_OPENGL_RENDER_PASS_H

#include <atta/graphics/renderPass.h>

namespace atta::graphics::gl {

class RenderPass final : public gfx::RenderPass {
  public:
    RenderPass(const RenderPass::CreateInfo& info);
    ~RenderPass();

    void begin() override;
    void end() override;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_RENDER_PASS_H
