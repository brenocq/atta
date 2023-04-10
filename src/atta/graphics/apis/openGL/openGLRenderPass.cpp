//--------------------------------------------------
// Atta Graphics Module
// openGLRenderPass.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/openGL/openGLRenderPass.h>

namespace atta::graphics {

OpenGLRenderPass::OpenGLRenderPass(const RenderPass::CreateInfo& info) : RenderPass(info) {}

OpenGLRenderPass::~OpenGLRenderPass() {}

void OpenGLRenderPass::begin() { _framebuffer->bind(true); }

void OpenGLRenderPass::end() { _framebuffer->unbind(); }

} // namespace atta::graphics
