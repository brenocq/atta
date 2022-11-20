//--------------------------------------------------
// Atta UI Module
// graphicsModuleWindow.h
// Date: 2022-10-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_LAYERS_EDITOR_WINDOWS_GRAPHICS_MODULE_WINDOW_H
#define ATTA_UI_LAYERS_EDITOR_WINDOWS_GRAPHICS_MODULE_WINDOW_H

#include <atta/graphics/interface.h>
#include <atta/ui/layers/editor/window.h>

namespace atta::ui {

class GraphicsModuleWindow : public Window<GraphicsModuleWindow> {
  private:
    GraphicsModuleWindow();
    void renderImpl();

    friend Window<GraphicsModuleWindow>;
};

} // namespace atta::ui

#endif // ATTA_UI_LAYERS_EDITOR_WINDOWS_GRAPHICS_MODULE_WINDOW_H
