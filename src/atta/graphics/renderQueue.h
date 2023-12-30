//--------------------------------------------------
// Atta Graphics Module
// renderQueue.h
// Date: 2023-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDER_QUEUE_H
#define ATTA_GRAPHICS_RENDER_QUEUE_H

namespace atta::graphics {

class RenderQueue {
  public:
    RenderQueue();
    virtual ~RenderQueue() = default;

    virtual void begin() = 0;
    virtual void end() = 0;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDER_QUEUE_H
