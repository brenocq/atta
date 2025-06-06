//--------------------------------------------------
// Atta Graphics Module
// renderer.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_RENDERER_H
#define ATTA_GRAPHICS_RENDERER_H

#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/framebuffer.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Renderer {
  public:
    Renderer(const char* name) : _name(StringId(name)), _renderDrawer(true), _renderSelected(true) {}
    virtual ~Renderer() = default;

    virtual void render(std::shared_ptr<Camera> camera) = 0;
    virtual void resize(uint32_t width, uint32_t height) = 0;

    virtual void* getImGuiTexture() const = 0;
    virtual std::shared_ptr<Framebuffer> getFramebuffer() = 0;

    uint32_t getWidth() const { return _width; }
    uint32_t getHeight() const { return _height; }
    std::string getName() const { return _name.getString(); }
    StringId getSID() const { return _name; }

    void setRenderDrawer(bool renderDrawer) { _renderDrawer = renderDrawer; }
    void setRenderSelected(bool renderSelected) { _renderSelected = renderSelected; }

  protected:
    StringId _name;
    uint32_t _width;
    uint32_t _height;
    bool _renderDrawer;
    bool _renderSelected;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_RENDERER_H
