//--------------------------------------------------
// Atta UI Module
// viewportWindows.h
// Date: 2021-12-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_WINDOWS_VIEWPORT_VIEWPORT_WINDOWS_H
#define ATTA_UI_WINDOWS_VIEWPORT_VIEWPORT_WINDOWS_H

#include <atta/component/interface.h>
#include <atta/graphics/compute/entityClick.h>
#include <atta/ui/widgets/gizmo.h>
#include <atta/ui/windows/viewport/viewport.h>

namespace atta::ui {

class ViewportWindows {
  public:
    void startUp();
    void shutDown();

    void renderViewports();
    void renderUI();

    const std::vector<std::shared_ptr<Viewport>>& getViewports() const;
    void clearViewports();
    void addViewport(std::shared_ptr<Viewport> viewport);
    void removeViewport(std::shared_ptr<Viewport> viewport);
    void openViewportModal(StringId sid);
    bool getViewportRendering() const;
    void setViewportRendering(bool viewportRendering);

  private:
    void createDefaultViewports();
    void addBasicShapePopup();
    void renderModals();

    std::vector<std::shared_ptr<Viewport>> _viewports;     ///< Current viewports to be rendered
    std::vector<std::shared_ptr<Viewport>> _viewportsNext; ///< Being used for now to update the viewports in the next frame without breaking imgui
    bool _swapViewports = false;                           ///< If _viewports should be swapped
    float _viewportFPS = 30.0;                             ///< Desired viewport FPS (UI module handles the viewport rendering)
    bool _viewportRendering = true;                        ///< If should render the viewport

    // Gizmo
    Gizmo _gizmo;

    // Compute
    std::unique_ptr<gfx::EntityClick> _computeEntityClick;

    // Modals
    std::map<StringId, bool> _openModals; ///< Open modals for each viewport
};

} // namespace atta::ui

#endif // ATTA_UI_WINDOWS_VIEWPORT_VIEWPORT_WINDOWS_H
