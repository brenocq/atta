//--------------------------------------------------
// Atta Graphics Module
// viewport.h
// Date: 2021-09-14
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_VIEWPORT_H
#define ATTA_GRAPHICS_VIEWPORT_H

#include <atta/file/serializer/serializable.h>
#include <atta/graphics/cameras/camera.h>
#include <atta/graphics/renderers/renderer.h>
#include <atta/utils/stringId.h>

namespace atta::graphics {

class Viewport final : public file::Serializable {
  public:
    struct CreateInfo {
        StringId sid = StringId("Unnamed viewport");
        std::shared_ptr<Renderer> renderer = nullptr;
        std::shared_ptr<Camera> camera = nullptr;
        std::string name;
    };
    Viewport();
    Viewport(CreateInfo info);
    ~Viewport();

    void render();
    void resize(uint32_t width, uint32_t height);

    StringId getSID() { return _sid; }
    std::string getName() { return _name; }
    uint32_t getWidth() const { return _renderer->getWidth(); }
    uint32_t getHeight() const { return _renderer->getHeight(); }
    void* getImGuiTexture() const { return _renderer->getImGuiTexture(); }

    std::shared_ptr<Renderer> getRenderer() const { return _renderer; }
    std::shared_ptr<Camera> getCamera() const { return _camera; }

    void setRenderer(std::shared_ptr<Renderer> renderer) { _newRenderer = renderer; }
    void setCamera(std::shared_ptr<Camera> camera) { _camera = camera; }

    // Serializable
    void serialize(std::ostream& os) override;
    void deserialize(std::istream& is) override;
    unsigned getSerializedSize() { return file::Serializable::getSerializedSize(this); }
    // UI
    void renderUI();

  private:
    StringId _sid;
    std::shared_ptr<Renderer> _renderer;
    // If want to change the renderer, store new renderer in _newRenderer until next frame to avoid destroying current renderer that can still be in
    // use
    std::shared_ptr<Renderer> _newRenderer;
    std::shared_ptr<Camera> _camera;

    // UI
    std::string _name;
    std::vector<char> _inputText;
};

} // namespace atta::graphics

#endif // ATTA_GRAPHICS_VIEWPORT_H
