//--------------------------------------------------
// Atta Graphics System
// openGLRenderPass.cpp
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/rendererAPIs/openGL/openGLRenderPass.h>

namespace atta
{
    OpenGLRenderPass::OpenGLRenderPass(const RenderPass::CreateInfo& info):
        RenderPass(info)
    {

    }

    OpenGLRenderPass::~OpenGLRenderPass()
    {

    }

    void OpenGLRenderPass::begin(bool clear)
    {
        _framebuffer->bind(clear);
    }

    void OpenGLRenderPass::end()
    {
        _framebuffer->unbind();
    }
}
