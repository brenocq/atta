//--------------------------------------------------
// Atta Graphics Module
// renderPass.h
// Date: 2021-09-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDER_PASS_H
#define ATTA_GRAPHICS_RENDER_PASS_H
#include <atta/graphics/framebuffer.h>
#include <atta/graphics/renderQueue.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class RenderPass {
  public:
    struct CreateInfo {
        std::shared_ptr<Framebuffer> framebuffer;
        StringId debugName = StringId("Unnamed Render Pass");
    };

    RenderPass(const CreateInfo& info);
    virtual ~RenderPass() = default;

    virtual void begin(std::shared_ptr<RenderQueue> renderQueue) = 0;
    virtual void end() = 0;

    std::shared_ptr<Framebuffer> getFramebuffer() const { return _framebuffer; }
    std::shared_ptr<RenderQueue> getRenderQueue() const { return _renderQueue; }

  protected:
    std::shared_ptr<Framebuffer> _framebuffer;
    std::shared_ptr<RenderQueue> _renderQueue;

    const StringId _debugName;
    friend Framebuffer;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDER_PASS_H
