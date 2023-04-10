//--------------------------------------------------
// Atta Graphics Module
// renderPass.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/renderPass.h>

namespace atta::graphics::gl {

RenderPass::RenderPass(const RenderPass::CreateInfo& info) : gfx::RenderPass(info) {}

RenderPass::~RenderPass() {}

void RenderPass::begin() { _framebuffer->bind(true); }

void RenderPass::end() { _framebuffer->unbind(); }

} // namespace atta::graphics::gl
