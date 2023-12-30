//--------------------------------------------------
// Atta Graphics Module
// renderQueue.h
// Date: 2023-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_APIS_OPENGL_RENDER_QUEUE_H
#define ATTA_GRAPHICS_APIS_OPENGL_RENDER_QUEUE_H

#include <atta/graphics/renderQueue.h>

namespace atta::graphics::gl {

class RenderQueue final : public gfx::RenderQueue {
  public:
    void begin() override;
    void end() override;
};

} // namespace atta::graphics::gl

#endif // ATTA_GRAPHICS_APIS_OPENGL_RENDER_QUEUE_H
